#include "../util/logger.h"
#include "../util/fifo_escritura.h"
#include "../util/defines.h"
#include "../util/signal_wait.h"
#include <unistd.h>

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("KID");

	pid_t myPid = getpid();
	Logger::log("%s my pid es %d",  "KID", myPid);

	FifoEscritura ticket(SELLER_FIFO);
	ticket.abrir();
	ticket.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
	ticket.cerrar();

	Logger::log("KID: compro entrada");

	/*int sig =*/ Signal::waitSignal(SIGUSR1);

	Logger::log("KID: Hago la fila");

	KidPosition position;
	position.pid = myPid;
	position.pos = 1;

	FifoEscritura queue(QUEUE_FIFO);
	queue.abrir();
	queue.escribir(static_cast<const void*> (&position), sizeof(KidPosition));
	queue.cerrar();

	Logger::log("KID: Espero a la calecita");

	/*int sig =*/ Signal::waitSignal(SIGUSR1);

	Logger::log("KID: End");
	return 0;
}
