#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/lock_file.h"
#include "../util/memoria_compartida2.h"
#include "../util/env_config.h"

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("SELLER");
	int ticket_cost = Config::getInt(ENVIROMENT_TICKET_COST, DEFAULT_TICKET_COST);

	FifoLectura tickets(SELLER_FIFO);
	tickets.abrir(true);
	ssize_t bytesLeidos;

	pid_t kidPid = 0;
	LockFile lock(MONEY_BOX);
	int recaudacion = 0;

	MemoriaCompartida2<int> box(MONEY_BOX, MONEY_BOX_CHAR);
	lock.tomarLock();
	box.escribir(recaudacion);
	lock.liberarLock();

	for(;;){
		bytesLeidos = tickets.leer(static_cast<void*> (&kidPid), sizeof(pid_t));
		if(bytesLeidos != sizeof(pid_t)){
			continue;
		}
		
		Logger::log("%s pid leido: %d", "SELLER", kidPid);
		lock.tomarLock();
		recaudacion += ticket_cost;
		box.escribir(recaudacion);
		lock.liberarLock();

		kill(kidPid, SIGUSR1);
	}

	tickets.cerrar();

	Logger::log("SELLER: END");
	return 0;
}
