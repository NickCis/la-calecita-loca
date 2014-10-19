#include "semaforo.h"

using std::string;

Semaforo::Semaforo(const string& nombre, int semnum, int semcant) : semnum(semnum) {
	this->init(nombre, 'a', semcant);
}

Semaforo::Semaforo(const string& nombre, char letter, int semnum, int semcant) : semnum(semnum) {
	this->init(nombre, letter, semcant);
}

Semaforo::~Semaforo() {
}

void Semaforo::init(const string& nombre, char letter, int semcant) {
	key_t clave = ftok(nombre.c_str(), letter);
	this->id = semget(clave, semcant, 0666 | IPC_CREAT );
	if(this->id == -1)
		throw string("Error semget");
}

int Semaforo::semctl(int cmd, int arg) const {
	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = arg;
	return ::semctl(this->id, this->semnum, cmd, init);
}

int Semaforo::semctl(int cmd) const {
	return ::semctl(this->id, this->semnum, cmd);
}

int Semaforo::setVal(int i) const {
	return this->semctl(SETVAL, i);
}

int Semaforo::getVal() const {
	return this->semctl(GETVAL);
}

int Semaforo::semop(short sem_op, short sem_flg) const{
	struct sembuf op;

	op.sem_num = this->semnum; // numero de semaforo
	op.sem_op  = sem_op; // restar 1 al semaforo
	op.sem_flg = sem_flg;

	// El ultimo uno es para una sola operacion
	return ::semop(this->id, &op, 1);
}

int Semaforo::p(int i, short sem_flg) const {
	return this->semop(-i, sem_flg);
}

int Semaforo::v(int i, short sem_flg) const {
	return this->semop(i, sem_flg);
}

int Semaforo::wait() const {
	return this->semop(0, SEM_UNDO);
}

int Semaforo::eliminar() const {
	return semctl(IPC_RMID);
}
