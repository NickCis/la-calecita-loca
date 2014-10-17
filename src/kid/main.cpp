#include "../util/logger.h"
#include "../util/fifo_escritura.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/calesita.h"
#include <unistd.h>

#include <sstream>

using std::stringstream;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);
	Logger::log("Inicio");

	pid_t myPid = getpid();

	stringstream ss;
	ss << KID_FIFO << myPid;

	int posicionDeseada = 3;

	FifoEscritura ticket(SELLER_FIFO);
	ticket.abrir();
	ticket.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
	ticket.cerrar();

	Logger::log("compro entrada");

	pid_t otherPid;
	FifoLectura kidIn(ss.str());
	kidIn.abrir(true);

	ssize_t bytesLeidos = kidIn.leer(static_cast<void*> (&otherPid), sizeof(pid_t));
	if(bytesLeidos != sizeof(pid_t)){
		Logger::log(" error comprando la entrada");
		return -1;
	}

	Logger::log("Hago la fila para la calesita");

	FifoEscritura queue(QUEUE_FIFO);
	queue.abrir();
	queue.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
	queue.cerrar();

	Logger::log("Espero a la calesita");

	bytesLeidos = kidIn.leer(static_cast<void*> (&otherPid), sizeof(pid_t));
	if(bytesLeidos != sizeof(pid_t)){
		Logger::log("error esperando a la calesita");
		return -1;
	}

	CalesitaUsuario calesita;
	calesita.ocuparPosicion(posicionDeseada);

	if(calesita.divertirme()){ //TODO: manejar error
	}

	kidIn.cerrar();
	kidIn.eliminar();

	Logger::log("End");

	return 0;
}
