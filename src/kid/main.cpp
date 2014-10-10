#include "../util/logger.h"
#include "../util/fifo_escritura.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/calecita.h"
#include <unistd.h>

#include <sstream>

using std::stringstream;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("KID");

	pid_t myPid = getpid();

	stringstream ss;
	ss << KID_FIFO << myPid;

	int posicionDeseada = 3;

	FifoEscritura ticket(SELLER_FIFO);
	ticket.abrir();
	ticket.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
	ticket.cerrar();

	Logger::log("KID: compro entrada");

	pid_t otherPid;
	FifoLectura kidIn(ss.str());
	kidIn.abrir(true);

	ssize_t bytesLeidos = kidIn.leer(static_cast<void*> (&otherPid), sizeof(pid_t));
	if(bytesLeidos != sizeof(pid_t)){
		Logger::log("KID: error comprando la entrada");
		return -1;
	}

	Logger::log("KID: Hago la fila para la calecita");

	FifoEscritura queue(QUEUE_FIFO);
	queue.abrir();
	queue.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
	queue.cerrar();

	Logger::log("KID: Espero a la calecita");

	bytesLeidos = kidIn.leer(static_cast<void*> (&otherPid), sizeof(pid_t));
	if(bytesLeidos != sizeof(pid_t)){
		Logger::log("KID: error esperando a la calecita");
		return -1;
	}

	Logger::log("KID: Entro a la calecita y quiero lugar %d", posicionDeseada);

	Calecita calecita;
	calecita.tomarLock();
	int posicionObtenida = calecita.ocuparPosicion(posicionDeseada);
	Logger::log("KID: Entre a la calecita y ocupe la posicion %d", posicionObtenida);
	calecita.liberarLock();

	LockFile imOut(KIDS_OUT_LOCK);
	imOut.tomarLock(false);

	LockFile exitLock(SALIDA_LOCK);
	exitLock.tomarLock();

	Logger::log("KID: Es mi turno de salir");
	exitLock.liberarLock();

	kidIn.cerrar();
	kidIn.eliminar();

	Logger::log("KID: End");

	imOut.liberarLock();
	return 0;
}
