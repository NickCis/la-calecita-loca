#include "calecita.h"
#include "../util/defines.h"
#include "../util/env_config.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"

Calecita::Calecita() : shm(NULL), size(0), cantidadPosiciones(0), lock(POSICIONES_CALECITA), posiciones(NULL) {
	this->cantidadPosiciones = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS);
	this->size = sizeof(pid_t) * this->cantidadPosiciones;
	this->shm = new MemoriaCompartida3<pid_t>(POSICIONES_CALECITA, POSICIONES_CALECITA_CHAR, this->size);
	this->posiciones = new pid_t[this->cantidadPosiciones];
}

Calecita::~Calecita() {
	delete this->shm;
	delete[] this->posiciones;
}

int Calecita::tomarLock(){
	return this->lock.tomarLock();
}

int Calecita::liberarLock(){
	return this->lock.liberarLock();
}

int Calecita::ocuparPosicion(int pos){
	pid_t myPid = getpid();
	pos = pos % this->cantidadPosiciones;
	this->shm->leer(this->posiciones);

	if(this->posiciones[pos]){
		for(pos = 0; pos < this->cantidadPosiciones; pos++)
			if(!this->posiciones[pos])
				break;

		if(pos >= this->cantidadPosiciones || this->posiciones[pos])
			return -1;
	}

	this->posiciones[pos] = myPid;
	this->shm->escribir(this->posiciones);


	FifoEscritura dentroCalecita(DENTRO_CALECITA_FIFO);
	dentroCalecita.abrir();
	dentroCalecita.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
	dentroCalecita.cerrar();

	return pos;
}

void Calecita::clear(){
	this->tomarLock();
	memset(this->posiciones, 0, this->size);
	this->shm->escribir(this->posiciones);
	this->liberarLock();
}

void Calecita::esperarEntradaChicos(int cantChicos){
	FifoLectura dentro(DENTRO_CALECITA_FIFO);
	pid_t kid;
	dentro.abrir(true);
	while(cantChicos-- > 0 ){
		dentro.leer(static_cast<void*> (&kid), sizeof(pid_t));
	}

	dentro.cerrar();
	dentro.eliminar();
}


