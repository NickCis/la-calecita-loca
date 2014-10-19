#include "../util/logger.h"
#include "../util/fifo_escritura.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/calesita.h"
#include "../util/cola.h"
#include <unistd.h>

#include <sstream>

using std::string;
using std::stringstream;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);
	Logger::log("Inicio");

	try{
		pid_t myPid = getpid();

		stringstream ss;
		ss << KID_FIFO << myPid;

		int posicionDeseada = 3;

		ColaEscritura<pid_t> colaTickets(SELLER_SEM, SELLER_FIFO);
		EXIT_ON_TRUE(colaTickets.write(&myPid));

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
		EXIT_ON_TRUE(calesita.ocuparPosicion(posicionDeseada) < 0);

		EXIT_ON_TRUE(calesita.divertirme());

		kidIn.cerrar();
		kidIn.eliminar();
	} catch(string &str){
		Logger::log("Exepcion: %s", str.c_str());
		return -1;
	} catch(...){
		Logger::log("Exepcion");
		return -1;
	}
	Logger::log("End");
	return 0;
}
