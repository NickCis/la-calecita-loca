#include "../util/logger.h"
#include "../util/env_config.h"
#include "../util/fifo_escritura.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/calesita.h"
#include "../util/cola.h"
#include <unistd.h>
#include <errno.h>

#include <memory>

using std::string;
using std::unique_ptr;

int main(int argc __attribute__ ((unused)), char* argv[]){
	Logger::setName(argv[0]);
	Logger::log("Inicio");

	pid_t myPid = getpid(),
		otherPid;

	int posicionDeseada = 3;

	CREATE_UNIQUE_PTR(kidIn, FifoLectura, new FifoLectura(Config::buildKidFifoPath(myPid)));
	kidIn->unlink();

	{
		CREATE_UNIQUE_PTR(colaTickets, ColaEscritura<pid_t>, new ColaEscritura<pid_t>(Config::getBinPath(SELLER_BIN), SELLER_FIFO));
		EXIT_ON_TRUE(colaTickets->write(&myPid));
	}

	Logger::log("Hago fila para comprar entrada");

	EXIT_ON_TRUE_EXE(kidIn->mknod() && errno != EEXIST, {
		Logger::log("Error %d '%s'", errno, strerror(errno));
	});
	EXIT_ON_TRUE(kidIn->open());
	kidIn->read(static_cast<void*> (&otherPid), sizeof(pid_t));
	EXIT_ON_TRUE(kidIn->close());
	EXIT_ON_TRUE(kidIn->unlink());

	Logger::log("Compre entrada");

	Logger::log("Hago la fila para la calesita");

	{
		CREATE_UNIQUE_PTR(colaCalesita, ColaEscritura<pid_t>, new ColaEscritura<pid_t>(Config::getBinPath(CALESITA_BIN), QUEUE_CALESITA_FIFO, (int) CalesitaSemaforos::colaCalesita, (int) CalesitaSemaforos::totalSemaforos));
		EXIT_ON_TRUE(colaCalesita->write(&myPid));
	}

	EXIT_ON_TRUE_EXE(kidIn->mknod() && errno != EEXIST, {
		Logger::log("Error %d '%s'", errno, strerror(errno));
	});
	EXIT_ON_TRUE(kidIn->open());
	kidIn->read(static_cast<void*> (&otherPid), sizeof(pid_t));
	EXIT_ON_TRUE(kidIn->close());
	EXIT_ON_TRUE(kidIn->unlink());

	Logger::log("Voy a entrar a la calesita");

	CREATE_UNIQUE_PTR(calesita, CalesitaUsuario, new CalesitaUsuario());

	EXIT_ON_TRUE(calesita->ocuparPosicion(posicionDeseada) < 0);

	EXIT_ON_TRUE(calesita->divertirme());

	Logger::log("End");
	return 0;
}
