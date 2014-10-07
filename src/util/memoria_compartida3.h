#ifndef MEMORIACOMPARTIDA3_H_
#define MEMORIACOMPARTIDA3_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>

template <class T> class MemoriaCompartida3 {

private:
	int shmId;
	T* ptrDatos;
	size_t size;

	int	cantidadProcesosAdosados() const;

public:
	MemoriaCompartida3 (size_t s);
	void crear ( const std::string& archivo,const char letra );
	void liberar ();

	MemoriaCompartida3 ( const std::string& archivo, const char letra, size_t s );
	MemoriaCompartida3 ( const MemoriaCompartida3& origen );
	~MemoriaCompartida3 ();
	MemoriaCompartida3<T>& operator= ( const MemoriaCompartida3& origen );
	void escribir ( const T* ptr );
	void leer (T* ptr) const;
};

template <class T> MemoriaCompartida3<T>::MemoriaCompartida3 ( size_t s):shmId(0),ptrDatos(NULL), size(s) {
}

template <class T> void MemoriaCompartida3<T>::crear ( const std::string& archivo,const char letra ) {
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave, this->size, 0644|IPC_CREAT );

		if ( this->shmId > 0 ) {
			void* tmpPtr = shmat ( this->shmId,NULL,0 );
			if ( tmpPtr != (void*) -1 ) {
				this->ptrDatos = static_cast<T*> (tmpPtr);
			} else {
				std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error en shmget(): ") + std::string(strerror(errno));
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> void MemoriaCompartida3<T>::liberar() {
	int errorDt = shmdt ( (void *) this->ptrDatos );

	if ( errorDt != -1 ) {
		int procAdosados = this->cantidadProcesosAdosados ();
		if ( procAdosados == 0 ) {
			shmctl ( this->shmId,IPC_RMID,NULL );
		}
	} else {
		std::string mensaje = std::string("Error en shmdt(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> MemoriaCompartida3<T>::MemoriaCompartida3 ( const std::string& archivo,const char letra, size_t s ):shmId(0),ptrDatos(NULL), size(s) {
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave,this->size,0644|IPC_CREAT );

		if ( this->shmId > 0 ) {
			void* tmpPtr = shmat ( this->shmId,NULL,0 );
			if ( tmpPtr != (void*) -1 ) {
				this->ptrDatos = static_cast<T*> (tmpPtr);
			} else {
				std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error en shmget(): ") + std::string(strerror(errno));
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> MemoriaCompartida3<T>::MemoriaCompartida3 ( const MemoriaCompartida3& origen ):shmId(origen.shmId), size(origen.size) {
	void* tmpPtr = shmat ( origen.shmId,NULL,0 );

	if ( tmpPtr != (void*) -1 ) {
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> MemoriaCompartida3<T>::~MemoriaCompartida3 () {
	int errorDt = shmdt ( static_cast<void*> (this->ptrDatos) );

	if ( errorDt != -1 ) {
		int procAdosados = this->cantidadProcesosAdosados ();
		if ( procAdosados == 0 ) {
			shmctl ( this->shmId,IPC_RMID,NULL );
		}
	} else {
		std::cerr << "Error en shmdt(): " << strerror(errno) << std::endl;
	}
}

template <class T> MemoriaCompartida3<T>& MemoriaCompartida3<T>::operator= ( const MemoriaCompartida3& origen ) {
	this->shmId = origen.shmId;
	void* tmpPtr = shmat ( this->shmId,NULL,0 );

	if ( tmpPtr != (void*) -1 ) {
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
		throw mensaje;
	}

	return *this;
}

template <class T> void MemoriaCompartida3<T>::escribir ( const T* ptr ) {
	memcpy(this->ptrDatos, ptr, this->size);
}

template <class T> void MemoriaCompartida3<T>::leer (T* ptr) const {
	memcpy(ptr, this->ptrDatos, this->size);
}

template <class T> int MemoriaCompartida3<T> :: cantidadProcesosAdosados () const {
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );
	return estado.shm_nattch;
}

#endif
