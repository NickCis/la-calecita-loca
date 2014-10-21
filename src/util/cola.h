#include <string>
#include "../util/semaforo.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"

class Cola {
	protected:
		/** Semaforo usado para ver cuando entraron todos los chicos a la calesita
		 */
		Semaforo sem;

		Cola(std::string &semPath, int semnum = 0, int semcant = 1) : sem(semPath, semnum, semcant){}
		~Cola(){}
};

class ColaPrinter : public Cola {
	public:
		int getVal(){
			return sem.getVal();
		}

		ColaPrinter(std::string semPath) : Cola(semPath){
		}
};

template <typename T> class ColaEscritura : public Cola {
	protected:
		FifoEscritura fifo;

	public:
		ColaEscritura(std::string semPath, std::string fifoPath, int semnum = 0, int semcant = 1) : Cola(semPath, semnum, semcant), fifo(fifoPath){
			if(fifo.open())
				throw std::string("No se pudo abrir fifo");
		}

		~ColaEscritura(){
			fifo.close();
		}

		/** Escribe en la cola
		 * Escribe y hace operaciones con semaforo
		 * @param buf valor que escribe
		 * @return 0 ok, resto error
		 */
		int write(T* buf){
			int ret = 0;

			if((ret = fifo.write((void*) buf, sizeof(T))) <= 0)
				return ret;

			sem.p(1, IPC_NOWAIT);

			return 0;
		}
};


template <typename T> class ColaLectura : public Cola {
	protected:
		FifoLectura fifo;

	public:
		ColaLectura(std::string semPath, std::string fifoPath, int semnum = 0, int semcant = 1) : Cola(semPath, semnum, semcant), fifo(fifoPath){
			if(sem.setVal(1))
				throw std::string("No se pudo setear el semaforo");

			fifo.unlink();

			if(fifo.mknod())
				throw std::string("Error con mknod");

			if(fifo.open())
				throw std::string("No se pudo abrir fifo");

		}

		~ColaLectura(){
			fifo.close();
			fifo.unlink();
			sem.eliminar();
		}

		/** Lee de la cola. Si no hay nada para leer espera a que algo halla
		 * @param dst destino a escribir el valor leido
		 * @return 0 ok, resto error
		 */
		int read(T* dst){
			if(!dst)
				return -1;

			ssize_t size = sizeof(T),
				bytesLeidos = fifo.read(dst, size);

			if(bytesLeidos == size)
				return 0;

			if(bytesLeidos == 0){
				if(sem.v())
					return -1;
				sem.wait();
				return read(dst);
			}

			return -1;
		}
};
