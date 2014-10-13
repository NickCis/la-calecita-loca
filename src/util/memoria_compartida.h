#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#define SHM_OK			 0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>


/** Clase para manejar memoria compartida
 */
template <class T> class MemoriaCompartida {

private:
	/** id de la shm
	 */
	int shmId;
	/** Puntero al mapeo de la shm
	 */
	T* ptrDatos;

	/** Metodo que devuelve la cantidad de procesos adosados a la memoria compartida.
	 * @return cantidad de procesos adosados
	 */
	int cantidadProcesosAdosados () const;

public:
	MemoriaCompartida ();
	~MemoriaCompartida ();

	/** Crea la memoria compartida.
	 * @param archivo: path del archivo a utilizar
	 * @param letra: letra a utilizar
	 * @return ERROR_FTOK si hubo error en la creacion de la clave, ERROR_SHMGET si hubo error en shmget, ERROR_SHMAT si hubo error en shmat o SHM_OK si todo salio bien.
	 */
	int crear ( const std::string& archivo,const char letra );

	/** Liberar se desadosa de la memoria compartida.
	 */
	void liberar ();

	/** Escribe en la memoria compartida
	 * @param dato: referencia al dato a escribir
	 */
	void escribir ( const T& dato );

	/** Lee de la memoria compartida.
	 * @return dato leido
	 */
	T leer () const;

};

template <class T> MemoriaCompartida<T> :: MemoriaCompartida() : shmId(0), ptrDatos(NULL) {
}

template <class T> MemoriaCompartida<T> :: ~MemoriaCompartida() {
}

template <class T> int MemoriaCompartida<T> :: crear ( const std::string& archivo,const char letra ) {

	// generacion de la clave
	key_t clave = ftok ( archivo.c_str(),letra );
	if ( clave == -1 )
		return ERROR_FTOK;
	else {
		// creacion de la memoria compartida
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

		if ( this->shmId == -1 )
			return ERROR_SHMGET;
		else {
			// attach del bloque de memoria al espacio de direcciones del proceso
			void* ptrTemporal = shmat ( this->shmId,NULL,0 );

			if ( ptrTemporal == (void *) -1 ) {
				return ERROR_SHMAT;
			} else {
				this->ptrDatos = static_cast<T*> (ptrTemporal);
				return SHM_OK;
			}
		}
	}
}


template <class T> void MemoriaCompartida<T> :: liberar () {
	// detach del bloque de memoria
	shmdt ( static_cast<void*> (this->ptrDatos) );

	int procAdosados = this->cantidadProcesosAdosados ();

	if ( procAdosados == 0 ) {
		shmctl ( this->shmId,IPC_RMID,NULL );
	}
}

template <class T> void MemoriaCompartida<T> :: escribir ( const T& dato ) {
	* (this->ptrDatos) = dato;
}

template <class T> T MemoriaCompartida<T> :: leer () const {
	return ( *(this->ptrDatos) );
}

template <class T> int MemoriaCompartida<T> :: cantidadProcesosAdosados () const {
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );
	return estado.shm_nattch;
}


#endif /* MEMORIACOMPARTIDA_H_ */
