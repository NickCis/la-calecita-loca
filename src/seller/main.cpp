#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"
#include "../util/defines.h"
#include "../util/lock_file.h"
#include "../util/memoria_compartida2.h"
#include "../util/env_config.h"
#include "../util/cola.h"

#include <unistd.h>

#include <string>
#include <sstream>
#include <memory>

using std::string;
using std::unique_ptr;
using std::stringstream;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);

	int ticket_cost = Config::getInt(ENVIROMENT_TICKET_COST, DEFAULT_TICKET_COST),
		chicos = Config::getInt(ENVIROMENT_CANT_CHICOS, DEFAULT_CANT_CHICOS),
		recaudacion = 0;

	Logger::log("Inicio: costo de entrada: %d cantidad de chicos: %d", ticket_cost, chicos);
	pid_t myPid = getpid();
	pid_t kidPid = 0;

	CREATE_UNIQUE_PTR(lock, LockFile, new LockFile(MONEY_BOX));

	CREATE_UNIQUE_PTR(box, MemoriaCompartida2<int>, new MemoriaCompartida2<int>(MONEY_BOX, MONEY_BOX_CHAR));

	lock->tomarLock();
	box->escribir(recaudacion);
	lock->liberarLock();

	CREATE_UNIQUE_PTR(colaTickets, ColaLectura<pid_t>, new ColaLectura<pid_t>(SELLER_SEM, SELLER_FIFO));

	for(int i=0;i<chicos;i++){
		EXIT_ON_TRUE(colaTickets->read(&kidPid));

		Logger::log("Chico %d quiere comprar una entrada", kidPid);

		lock->tomarLock();
		recaudacion += ticket_cost;
		box->escribir(recaudacion);
		lock->liberarLock();

		stringstream ss;
		ss << KID_FIFO << kidPid;
		FifoEscritura chico(ss.str());
		chico.open();
		chico.write(static_cast<const void*> (&myPid), sizeof(pid_t));
		chico.close();

		Logger::log("chico %d le vendi una entrada", kidPid);
	}

	Logger::log("Vendi todas las entradas");
	Logger::log("Exit");
	return 0;
}
