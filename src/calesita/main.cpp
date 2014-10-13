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
	Logger::compileInfo("CALESITA");
	pid_t myPid = getpid();

	int cant_asientos = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS),
		t_vuelta = Config::getInt(ENVIROMENT_T_VUELTA, DEFAULT_TIEMPO_VUELTA),
		cant_chicos = Config::getInt(ENVIROMENT_CANT_CHICOS, DEFAULT_CANT_CHICOS);

	Logger::log("CALESITA: cantidad de asientos: %d tiempo de vuelta: %d cantidad de chicos: %d", cant_asientos, t_vuelta, cant_chicos);

	FifoLectura fila(QUEUE_FIFO);
	fila.abrir(true);
	ssize_t bytesLeidos;

	int cantidad = 0;

	pid_t kid;
	vector<pid_t> kids;

	Calesita calesita;

	LockFile exitLock(SALIDA_LOCK);
	LockFile kidsOut(KIDS_OUT_LOCK);

	for(;;){
		bytesLeidos = fila.leer(static_cast<void*> (&kid), sizeof(pid_t));
		if(bytesLeidos != sizeof(pid_t)){
			continue;
		}
		
		Logger::log("CALESITA: chico hace la cola %d", kid);

		kids.push_back(kid);

		if(++cantidad >= cant_asientos || cantidad >= cant_chicos){
			exitLock.tomarLock();

			Logger::log("CALESITA: ya estan todos los chicos. Limpio calesita.");
			calesita.clear();

			calesita.tomarLock();
			Logger::log("CALESITA: Dejo entrar a los chicos");

			for(vector<pid_t>::iterator it = kids.begin(); it != kids.end(); ++it){
				pid_t kidPid = (*it);
				stringstream ss;
				ss << KID_FIFO << kidPid;
				FifoEscritura chico(ss.str());
				chico.abrir();
				chico.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
				chico.cerrar();
			}

			Logger::log("CALESITA: Espero que entren todos los chicos");
			calesita.liberarLock();
			calesita.esperarEntradaChicos(cantidad);

			Logger::log("CALESITA: Inicia la vuelta s: %d", t_vuelta);

			sleep(t_vuelta);

			Logger::log("CALESITA: Termino la vuelta. Espero a que salgan los chicos");

			exitLock.liberarLock();

			kidsOut.tomarLock();
			Logger::log("CALESITA: Salieron todos los chicos");
			kidsOut.liberarLock();

			cant_chicos -= cantidad;
			if(cant_chicos == 0)
				break;
			cantidad = 0;
			kids.clear();
			calesita.liberarLock();
		}
	}

	fila.cerrar();
	fila.eliminar();

	Logger::log("CALESITA: Ya todos los chicos se divirtieron en mi. Salgo.");
	return 0;
}
