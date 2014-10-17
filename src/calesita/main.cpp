#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"
#include "../util/defines.h"
#include "../util/env_config.h"
#include "../util/calesita.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <vector>
#include <sstream>

using std::vector;
using std::stringstream;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);
	pid_t myPid = getpid();

	int cant_asientos = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS),
		cant_chicos = Config::getInt(ENVIROMENT_CANT_CHICOS, DEFAULT_CANT_CHICOS);

	Logger::log("Inicio: cantidad de asientos: %d cantidad de chicos: %d", cant_asientos, cant_chicos);

	FifoLectura fila(QUEUE_FIFO);
	fila.abrir(true);
	ssize_t bytesLeidos;

	int cantidad = 0;

	pid_t kid;
	vector<pid_t> kids;

	CalesitaControlador calesita;
	for(;;){
		bytesLeidos = fila.leer(static_cast<void*> (&kid), sizeof(pid_t));
		if(bytesLeidos != sizeof(pid_t)){
			continue;
		}

		Logger::log("chico hace la cola %d", kid);

		kids.push_back(kid);

		if(++cantidad >= cant_asientos || cantidad >= cant_chicos){

			Logger::log("estan todos los chicos para entrar");
			calesita.inicializarNuevaVuelta(cantidad);
			Logger::log("Dejo entrar a los chicos");

			for(vector<pid_t>::iterator it = kids.begin(); it != kids.end(); ++it){
				pid_t kidPid = (*it);
				stringstream ss;
				ss << KID_FIFO << kidPid;
				FifoEscritura chico(ss.str());
				chico.abrir();
				chico.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
				chico.cerrar();
			}

			if(calesita.esperarEntradaChicos()){ // TODO: manejar error
				Logger::log("error %d",  __LINE__);
			}

			if(calesita.darVuelta()){ //TODO: manejar error
				Logger::log("error %d",  __LINE__);
			}

			if(calesita.esperarSalidaChicos()){ //TODO: manejar error
				Logger::log("error %d",  __LINE__);
			}

			cant_chicos -= cantidad;
			if(cant_chicos == 0)
				break;
			cantidad = 0;
			kids.clear();
		}
	}

	fila.cerrar();
	fila.eliminar();

	Logger::log("Ya todos los chicos se divirtieron en mi. Salgo.");
	return 0;
}
