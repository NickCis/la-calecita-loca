#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"
#include "../util/defines.h"
#include "../util/env_config.h"
#include "../util/calesita.h"
#include "../util/cola.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <vector>
#include <sstream>
#include <memory>

using std::vector;
using std::string;
using std::unique_ptr;
using std::stringstream;

int main(int argc __attribute__ ((unused)), char* argv[]){
	Logger::setName(argv[0]);
	pid_t myPid = getpid();

	int cant_asientos = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS),
		cant_chicos = Config::getInt(ENVIROMENT_CANT_CHICOS, DEFAULT_CANT_CHICOS),
		cantidad = 0;

	Logger::log("Inicio: cantidad de asientos: %d cantidad de chicos: %d", cant_asientos, cant_chicos);

	CREATE_UNIQUE_PTR(colaCalesita, ColaLectura<pid_t>, new ColaLectura<pid_t>(Config::getBinPath(CALESITA_BIN), QUEUE_CALESITA_FIFO, (int) CalesitaSemaforos::colaCalesita, (int) CalesitaSemaforos::totalSemaforos));

	pid_t kid;

	CREATE_UNIQUE_PTR(calesita, CalesitaControlador, new CalesitaControlador());

	EXIT_ON_TRUE(calesita->inicializarNuevaVuelta(cantidad));

	for(;;){
		EXIT_ON_TRUE(colaCalesita->read(&kid));

		Logger::log("chico [%d] hace la cola", kid);

		stringstream ss;
		ss << KID_FIFO << kid;
		FifoEscritura chico(ss.str());
		chico.open();
		chico.write(static_cast<const void*> (&myPid), sizeof(pid_t));
		chico.close();

		Logger::log("dejo entrar a chico  [%d]", kid);

		if(++cantidad >= cant_asientos || cantidad >= cant_chicos){
			EXIT_ON_TRUE(calesita->esperarEntradaChicos());

			EXIT_ON_TRUE(calesita->darVuelta());

			EXIT_ON_TRUE(calesita->esperarSalidaChicos());

			cant_chicos -= cantidad;
			if(cant_chicos == 0)
				break;
			cantidad = 0;

			EXIT_ON_TRUE(calesita->inicializarNuevaVuelta(cantidad));
		}
	}

	Logger::log("Ya todos los chicos se divirtieron en mi. Salgo.");
	return 0;
}
