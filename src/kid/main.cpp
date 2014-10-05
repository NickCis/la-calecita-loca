#include "../util/logger.h"
#include "../util/fifo_escritura.h"
#include "../util/defines.h"
#include <unistd.h>

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("KID");

	pid_t myPid = getpid();
	Logger::log("%s my pid es %d",  "KID", myPid);

	FifoEscritura ticket(SELLER_FIFO);
	ticket.abrir();
	ticket.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
	ticket.cerrar();

	Logger::log("%s: End", "KID");
	return 0;
}
