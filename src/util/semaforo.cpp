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
}

int Semaforo::setVal(int i) const {
	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = i;
	return semctl(this->id, this->semnum, SETVAL, init);
}

int Semaforo::semop(short sem_op, short sem_flg) const{
	struct sembuf op;

	op.sem_num = this->semnum; // numero de semaforo
	op.sem_op  = sem_op; // restar 1 al semaforo
	op.sem_flg = sem_flg;

	// El ultimo uno es para una sola operacion
	return ::semop(this->id, &op, 1);
}

int Semaforo::p(int i) const {
	return this->semop(-i, SEM_UNDO);
}

int Semaforo::v(int i) const {
	return this->semop(i, SEM_UNDO);
}

int Semaforo::wait() const {
	return this->semop(0, SEM_UNDO);
}

int Semaforo::eliminar() const {
	return semctl(this->id, this->semnum, IPC_RMID);
}
