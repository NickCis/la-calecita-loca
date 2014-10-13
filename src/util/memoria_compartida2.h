#ifndef MEMORIACOMPARTIDA2_H_
#define MEMORIACOMPARTIDA2_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>

/** Clase para manejar memoria compartida
 */
template <class T> class MemoriaCompartida2 {

private:
	/** id de la shm
	 */
	int	shmId;
	/** Puntero al mapeo de la shm
	 */
	T* ptrDatos;

	/** Metodo que devuelve la cantidad de procesos adosados a la memoria compartida.
	 * @return cantidad de procesos adosados
	 */
	int	cantidadProcesosAdosados() const;

public:
	MemoriaCompartida2 ();

	/** Crea la memoria compartida.
	 * @param archivo: path del archivo a utilizar
	 * @param letra: letra a utilizar
	 * @return ERROR_FTOK si hubo error en la creacion de la clave, ERROR_SHMGET si hubo error en shmget, ERROR_SHMAT si hubo error en shmat o SHM_OK si todo salio bien.
	 */
	void crear ( const std::string& archivo,const char letra );

	/** Liberar se desadosa de la memoria compartida.
	 */
	void liberar ();

	/** Constructor que ejecuta metodo crear tmb, levanta excepcion si hay error.
	 * @param archivo: path del archivo a utilizar
	 * @param letra: letra a utilizar
	 * @see MemoriaCompartida2::crear
	 */
	MemoriaCompartida2 ( const std::string& archivo,const char letra );
	MemoriaCompartida2 ( const MemoriaCompartida2& origen );
	~MemoriaCompartida2 ();
	MemoriaCompartida2<T>& operator= ( const MemoriaCompartida2& origen );

	/** Escribe en la memoria compartida
	 * @param dato: referencia al dato a escribir
	 */
	void escribir ( const T& dato );

	/** Lee de la memoria compartida.
	 * @return dato leido
	 */
	T leer () const;

	/** Lee de la memoria compartida y lo escribe en ptr
	 * @param ptr: puntero en donde escribir (debe tener la memoria reservada).
	 */
	void leer (T* ptr) const;
};

template <class T> MemoriaCompartida2<T>::MemoriaCompartida2 ():shmId(0),ptrDatos(NULL) {
}

template <class T> void MemoriaCompartida2<T>::crear ( const std::string& archivo,const char letra ) {
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

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

template <class T> void MemoriaCompartida2<T>::liberar() {
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

template <class T> MemoriaCompartida2<T>::MemoriaCompartida2 ( const std::string& archivo,const char letra ):shmId(0),ptrDatos(NULL) {
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

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

template <class T> MemoriaCompartida2<T>::MemoriaCompartida2 ( const MemoriaCompartida2& origen ):shmId(origen.shmId) {
	void* tmpPtr = shmat ( origen.shmId,NULL,0 );

	if ( tmpPtr != (void*) -1 ) {
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> MemoriaCompartida2<T>::~MemoriaCompartida2 () {
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

template <class T> MemoriaCompartida2<T>& MemoriaCompartida2<T>::operator= ( const MemoriaCompartida2& origen ) {
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

template <class T> void MemoriaCompartida2<T>::escribir ( const T& dato ) {
	*(this->ptrDatos) = dato;
}

template <class T> T MemoriaCompartida2<T>::leer() const {
	return *(this->ptrDatos);
}

template <class T> void MemoriaCompartida2<T>::leer (T* ptr) const {
	memcpy(ptr, this->ptrDatos, sizeof(T));
}

template <class T> int MemoriaCompartida2<T> :: cantidadProcesosAdosados () const {
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );
	return estado.shm_nattch;
}

#endif
