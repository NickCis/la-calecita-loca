#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

class Semaforo {

private:
	int id;
	int semnum;
	void init(const std::string& nombre, char letter, int semcant);
	int semop(short sem_op, short sem_flg) const;
	int semctl(int cmd) const;
	int semctl(int cmd, int arg) const;

public:
	Semaforo(const std::string& nombre, int semnum = 0, int semcant = 1);
	Semaforo(const std::string& nombre, char letter, int semnum = 0, int semcant = 1);
	~Semaforo();

	/** Decrementa contador de semaforo
	 * @param i: en cuanto decrementa
	 * @return salida de semop, 0 ok, resto error
	 */
	int p(int i = 1, short sem_flg = SEM_UNDO) const; // decrementa

	/** Incrementa contador de semaforo
	 * @param i: en cuanto decrementa
	 * @return salida de semop, 0 ok, resto error
	 */
	int v(int i = 1, short sem_flg = SEM_UNDO) const; // incrementa

	/** Espera a que el semaforo sea cero
	 * @return salida de semop, 0 ok, resto error
	 */
	int wait() const;

	/** Setea el valor del semaforo.
	 * @param i valor a setear (0 por defecto)
	 * @return salida de semop, 0 ok, resto error
	 */
	int setVal(int i = 0) const;

	/** Devuelve el valor del semaforo.
	 * @return valor del semaforo
	 */
	int getVal() const;

	/**Elimina el semaforo
	 * @return salida de semctl, -1 mal, el resto habria que ver
	 */
	int eliminar() const;
};

#endif /* SEMAFORO_H_ */
