#include "calesita.h"
#include "../util/defines.h"
#include "../util/env_config.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"
#include "../util/logger.h"

using std::string;
using std::unique_ptr;

Calesita::Calesita() : size(0), cantidadPosiciones(0), posicionesLock(POSICIONES_CALESITA), exitLock(SALIDA_LOCK)  {
	this->cantidadPosiciones = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS);
	this->size = sizeof(pid_t) * this->cantidadPosiciones;

	string binPath = Config::getBinPath(CALESITA_BIN);

	dentroCalesita = unique_ptr<Semaforo> (new Semaforo(binPath, (int) CalesitaSemaforos::dentroCalesita, (int) CalesitaSemaforos::totalSemaforos));
	kidsOut = unique_ptr<Semaforo> (new Semaforo(binPath, (int) CalesitaSemaforos::kidsOut, (int) CalesitaSemaforos::totalSemaforos));
}

Calesita::~Calesita() {
}

CalesitaUsuario::CalesitaUsuario(){
}

CalesitaUsuario::~CalesitaUsuario(){
	kidsOut->p();
}

int CalesitaUsuario::lockPosicion(int pos){
	return this->posicionesLock.tomarLock(sizeof(pid_t), pos * sizeof(pid_t));
}

pid_t CalesitaUsuario::leerPosicion(int pos){
	pid_t pid;
	return this->posicionesLock.read(&pid, sizeof(pid_t), pos * sizeof(pid_t)) == sizeof(pid_t) ? pid : -1;
}

int CalesitaUsuario::escribirPosicion(int pos){
	pid_t pid = getpid();
	return this->posicionesLock.write(&pid, sizeof(pid_t), pos * sizeof(pid_t), SEEK_SET) == sizeof(pid_t) ? 0 : -1;
}

int CalesitaUsuario::ocuparPosicion(int pos){ //TODO: checkear errores
	Logger::log("Entro a la calesita y quiero lugar %d", pos);

	pos = pos % this->cantidadPosiciones;

	if(this->lockPosicion(pos))
		return -1;

	if(this->leerPosicion(pos) > 0){
		for(pos = 0; pos < this->cantidadPosiciones; pos++){
			this->posicionesLock.liberarLock();
			this->posicionesLock.tomarLock(pos);

			if(!this->leerPosicion(pos))
				break;
		}

		if(pos >= this->cantidadPosiciones){
			this->posicionesLock.liberarLock();
			return -1;
		}
	}

	this->escribirPosicion(pos);

	Logger::log("Entre a la calesita y ocupe la posicion %d.", pos);

	if(this->posicionesLock.liberarLock())
		return -1;

	if(kidsOut->v())
		return -1;

	if(this->dentroCalesita->p(1, IPC_NOWAIT))
		return -1;

	return pos;
}

int CalesitaUsuario::divertirme(){
	int ret = 0;
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
	dentroCalesita->eliminar();
	kidsOut->eliminar();
}

int CalesitaControlador::clear(){
	int ret = 0;
	if((ret = this->posicionesLock.tomarLock()))
		return ret;
	Logger::log("Limpio la calesita");
	//memset(this->posiciones, 0, this->size);
	//this->shm->escribir(this->posiciones); //TODO: Fijarse si esto puede salir con error
	for(int i=0; i < cantidadPosiciones; i++){
		pid_t pid = 0;
		this->posicionesLock.write(&pid, sizeof(pid_t), sizeof(pid_t) * i, SEEK_SET);
	}

	if((ret = this->posicionesLock.liberarLock()))
		return ret;

	return ret;
}

int CalesitaControlador::esperarEntradaChicos(){
	int ret = 0;

	Logger::log("Espero a que entren los chicos.");

	if((ret = dentroCalesita->wait()))
		return ret;

	Logger::log("Ya entraron todos los chicos.");
	return ret;
}

int CalesitaControlador::inicializarNuevaVuelta(int cantChicos){
	int ret = 0;
	if((ret = exitLock.tomarLock()))
		return ret;

	Logger::log("Van a entrar %d chicos", cantChicos);
	if((ret = dentroCalesita->setVal(cantChicos)))
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
	if((ret = kidsOut->wait()))
		return ret;

	Logger::log("Salieron todos los chicos");

	return ret;
}

