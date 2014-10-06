#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/lock_file.h"
#include "../util/memoria_compartida2.h"

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//XXX: parametrizar
#define TICKET_COST 5

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("SELLER");

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
		recaudacion += TICKET_COST;
		box.escribir(recaudacion);
		lock.liberarLock();

		kill(kidPid, SIGUSR1);
	}

	Logger::log("SELLER: bytesLeidos %d", bytesLeidos);

	tickets.cerrar();

	Logger::log("SELLER: END");
	return 0;
}
