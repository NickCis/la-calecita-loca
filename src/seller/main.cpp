#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"
#include "../util/defines.h"
#include "../util/lock_file.h"
#include "../util/memoria_compartida2.h"
#include "../util/env_config.h"

#include <unistd.h>

#include <sstream>

using std::stringstream;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);
	int ticket_cost = Config::getInt(ENVIROMENT_TICKET_COST, DEFAULT_TICKET_COST),
		chicos = Config::getInt(ENVIROMENT_CANT_CHICOS, DEFAULT_CANT_CHICOS);

	Logger::log("Inicio: costo de entrada: %dcantidad de chicos: %d", ticket_cost, chicos);

	FifoLectura tickets(SELLER_FIFO);
	tickets.abrir(true);
	ssize_t bytesLeidos;

	pid_t myPid = getpid();

	pid_t kidPid = 0;
	LockFile lock(MONEY_BOX);
	int recaudacion = 0;

	MemoriaCompartida2<int> box(MONEY_BOX, MONEY_BOX_CHAR);
	lock.tomarLock();
	box.escribir(recaudacion);
	lock.liberarLock();

	for(int i=0;i<chicos;i++){
		bytesLeidos = tickets.leer(static_cast<void*> (&kidPid), sizeof(pid_t));
		if(bytesLeidos != sizeof(pid_t)){
			continue;
		}
		
		Logger::log("Chico %d quiere comprar una entrada", kidPid);

		lock.tomarLock();
		recaudacion += ticket_cost;
		box.escribir(recaudacion);
		lock.liberarLock();

		stringstream ss;
		ss << KID_FIFO << kidPid;
		FifoEscritura chico(ss.str());
		chico.abrir();
		chico.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
		chico.cerrar();

		Logger::log("chico %d le vendi una entrada", kidPid);

	}

	tickets.cerrar();
	tickets.eliminar();

	Logger::log("Vendi todas las entradas. Salgo.");
	return 0;
}
