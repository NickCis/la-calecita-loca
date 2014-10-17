#include "calesita.h"
#include "../util/defines.h"
#include "../util/env_config.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"
#include "../util/logger.h"

Calesita::Calesita() : shm(NULL), size(0), cantidadPosiciones(0), lock(POSICIONES_CALESITA), posiciones(NULL), dentroCalesita(DENTRO_CALESITA), exitLock(SALIDA_LOCK), kidsOut(KIDS_OUT)  {
	this->cantidadPosiciones = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS);
	this->size = sizeof(pid_t) * this->cantidadPosiciones;
	this->shm = new MemoriaCompartida3<pid_t>(POSICIONES_CALESITA, POSICIONES_CALESITA_CHAR, this->size);
	this->posiciones = new pid_t[this->cantidadPosiciones];
}

Calesita::~Calesita() {
	delete this->shm;
	delete[] this->posiciones;
}

int Calesita::tomarLock(){
	return this->lock.tomarLock();
}

int Calesita::liberarLock(){
	return this->lock.liberarLock();
}

CalesitaUsuario::CalesitaUsuario(){
}

CalesitaUsuario::~CalesitaUsuario(){
	kidsOut.v();
}

int CalesitaUsuario::ocuparPosicion(int pos){ //TODO: checkear errores
	pid_t myPid = getpid();

	Logger::log("Entro a la calesita y quiero lugar %d", pos);

	pos = pos % this->cantidadPosiciones;

	this->tomarLock();

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

	sleep(1);
	Logger::log("Entre a la calesita y ocupe la posicion %d", pos);

	this->liberarLock();
	sleep(1);

	kidsOut.p();
	this->dentroCalesita.p();

	return pos;
}

int CalesitaUsuario::divertirme(){
	int ret = 0;
	Logger::log("Wiii, una vuelta =)");
	if((ret = exitLock.tomarLock()))
		return ret;

	Logger::log("Es mi turno de salir");
	if(( ret = exitLock.liberarLock()))
		return ret;

	return ret;
}

CalesitaControlador::CalesitaControlador(){
}

CalesitaControlador::~CalesitaControlador(){
	dentroCalesita.eliminar();
	kidsOut.eliminar();
}

int CalesitaControlador::clear(){
	int ret = 0;
	if((ret = this->tomarLock()))
		return ret;
	Logger::log("Limpio la SHM de calesita");
	memset(this->posiciones, 0, this->size);
	this->shm->escribir(this->posiciones); //TODO: Fijarse si esto puede salir con error

	if((ret = this->liberarLock()))
		return ret;

	return ret;
}

int CalesitaControlador::esperarEntradaChicos(){
	int ret = 0;

	Logger::log("Espero a que entren los chicos");

	if((ret = dentroCalesita.wait()))
		return ret;

	Logger::log("Ya entraron todos los chicos");
	return ret;
}

int CalesitaControlador::inicializarNuevaVuelta(int cantChicos){
	int ret = 0;
	if((ret = exitLock.tomarLock()))
		return ret;

	Logger::log("Van a entrar %d chicos", cantChicos);
	if((ret = dentroCalesita.setVal(cantChicos)))
		return ret;

	Logger::log("Limpio calesita");
	if((ret = this->clear()))
		return ret;

	return ret;
}

int CalesitaControlador::darVuelta(){
	int ret = 0,
		t_vuelta = Config::getInt(ENVIROMENT_T_VUELTA, DEFAULT_TIEMPO_VUELTA);
	Logger::log("Inicia la vuelta s: %d", t_vuelta);
	sleep(t_vuelta);
	Logger::log("Termino la vuelta.");
	if((ret = exitLock.liberarLock()))
		return ret;

	return ret;
}

int CalesitaControlador::esperarSalidaChicos(){
	int ret = 0;
	Logger::log("Espero salida de los chicos");
	if((ret = kidsOut.wait()))
		return ret;

	Logger::log("Salieron todos los chicos");

	return ret;
}

