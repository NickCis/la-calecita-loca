#include <string>
#include "../util/semaforo.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"

class Cola {
	protected:
		/** Semaforo usado para ver cuando entraron todos los chicos a la calesita
		 */
		Semaforo sem;

		Cola(std::string semPath) : sem(semPath){}
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
		ColaEscritura(std::string semPath, std::string fifoPath) : Cola(semPath), fifo(fifoPath){
			if(fifo.abrir())
				throw std::string("No se pudo abrir fifo");
		}

		~ColaEscritura(){
			if(fifo.cerrar())
				throw std::string("No se pudo cerrar fifo");
		}

		/** Escribe en la cola
		 * Escribe y hace operaciones con semaforo
		 * @param buf valor que escribe
		 * @return 0 ok, resto error
		 */
		int write(T* buf){
			int ret = 0;

			if((ret = fifo.escribir((void*) buf, sizeof(T))) <= 0)
				return ret;

			Logger::log("decremento semaforo: prev val %d", sem.getVal());
			if((ret = sem.p(1, IPC_NOWAIT))){
				//return ret;
				ret = 0;
			}

			sleep(1);
			Logger::log("pos val %d", sem.getVal());

			return ret;
		}
};


template <typename T> class ColaLectura : public Cola {
	protected:
		FifoLectura fifo;

	public:
		ColaLectura(std::string semPath, std::string fifoPath) : Cola(semPath), fifo(fifoPath){
			if(sem.setVal(1))
				throw std::string("No se pudo setear el semaforo");

			Logger::log("valor sem %d", sem.getVal());

			if(fifo.abrir())
				throw std::string("No se pudo abrir fifo");

			Logger::log("abri la fifo");
		}

		~ColaLectura(){
			if(fifo.cerrar())
				throw std::string("No se pudo cerrar fifo");

			if(fifo.eliminar())
				throw std::string("No se pudo eliminar fifo");
		}

		/** Lee de la cola. Si no hay nada para leer espera a que algo halla
		 * @param dst destino a escribir el valor leido
		 * @return 0 ok, resto error
		 */
		int read(T* dst){
			if(!dst)
				return -1;

			ssize_t size = sizeof(T);
			ssize_t bytesLeidos = fifo.leer(dst, size);
			if(bytesLeidos == size)
				return 0;

			if(bytesLeidos == 0){
				Logger::log("incremento semaforo prev val %d", sem.getVal());
				if(sem.v())
					return -1;
				Logger::log("pos val %d", sem.getVal());
				sem.wait();
				return read(dst);
			}

			Logger::log("bytesLeidos %d size %d *dst %d EOF %d", bytesLeidos, size, (char) *dst, EOF);

			return -1;
		}
};
