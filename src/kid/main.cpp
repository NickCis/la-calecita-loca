#include "../util/logger.h"
#include "../util/env_config.h"
#include "../util/fifo_escritura.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/calesita.h"
#include "../util/cola.h"
#include <unistd.h>
#include <errno.h>

#include <sstream>
#include <memory>

using std::string;
using std::unique_ptr;
using std::stringstream;

int main(int argc __attribute__ ((unused)), char* argv[]){
	Logger::setName(argv[0]);
	Logger::log("Inicio");

	pid_t myPid = getpid(),
		otherPid;
	stringstream ss;
	ss << KID_FIFO << myPid;

	int posicionDeseada = 3;

	CREATE_UNIQUE_PTR(kidIn, FifoLectura, new FifoLectura(ss.str()));
	kidIn->unlink();

	CREATE_UNIQUE_PTR(colaTickets, ColaEscritura<pid_t>, new ColaEscritura<pid_t>(SELLER_SEM, SELLER_FIFO));

	EXIT_ON_TRUE(colaTickets->write(&myPid));

	Logger::log("Hago fila para comprar entrada");

	EXIT_ON_TRUE(kidIn->mknod());
	EXIT_ON_TRUE(kidIn->open());
	EXIT_ON_TRUE_EXE(kidIn->read(static_cast<void*> (&otherPid), sizeof(pid_t)) != sizeof(pid_t), {
		Logger::log("errno %d '%s'", errno, strerror(errno));
	});
	EXIT_ON_TRUE(kidIn->close());
	EXIT_ON_TRUE(kidIn->unlink());

	Logger::log("Compre entrada");

	Logger::log("Hago la fila para la calesita");

	CREATE_UNIQUE_PTR(colaCalesita, ColaEscritura<pid_t>, new ColaEscritura<pid_t>(Config::getBinPath(CALESITA_BIN), QUEUE_CALESITA_FIFO, (int) CalesitaSemaforos::colaCalesita, (int) CalesitaSemaforos::totalSemaforos));
	EXIT_ON_TRUE(colaCalesita->write(&myPid));

	EXIT_ON_TRUE(kidIn->mknod());
	EXIT_ON_TRUE(kidIn->open());
	EXIT_ON_TRUE(kidIn->read(static_cast<void*> (&otherPid), sizeof(pid_t)) != sizeof(pid_t));
	EXIT_ON_TRUE(kidIn->close());
	EXIT_ON_TRUE(kidIn->unlink());

	Logger::log("Entro a la calesita");

	CREATE_UNIQUE_PTR(calesita, CalesitaUsuario, new CalesitaUsuario());

	EXIT_ON_TRUE(calesita->ocuparPosicion(posicionDeseada) < 0);

	EXIT_ON_TRUE(calesita->divertirme());

	Logger::log("End");
	return 0;
}
