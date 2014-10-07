#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/fifo_escritura.h"
#include "../util/defines.h"
#include "../util/env_config.h"
#include "../util/calecita.h"

#include <unistd.h>
#include <vector>
#include <sstream>

using std::vector;
using std::stringstream;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("CALECITA");
	pid_t myPid = getpid();

	int cant_asientos = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS),
		t_vuelta = Config::getInt(ENVIROMENT_T_VUELTA, DEFAULT_TIEMPO_VUELTA);

	Logger::log("CALECITA: cantidad de asientos: %d tiempo de vuelta: %d", cant_asientos, t_vuelta);

	FifoLectura fila(QUEUE_FIFO);
	fila.abrir(true);
	ssize_t bytesLeidos;

	int cantidad = 0;

	pid_t kid;
	vector<pid_t> kids;

	Calecita calecita;

	LockFile exitLock(SALIDA_LOCK);

	for(;;){
		bytesLeidos = fila.leer(static_cast<void*> (&kid), sizeof(pid_t));
		if(bytesLeidos != sizeof(pid_t)){
			continue;
		}
		
		Logger::log("CALECITA: chico hace la cola %d", kid);

		kids.push_back(kid);

		if(++cantidad >= cant_asientos){
			exitLock.tomarLock();

			cantidad = 0;
			Logger::log("CALECITA: ya estan todos los chicos. Limpio calecita.");
			calecita.clear();

			calecita.tomarLock();
			Logger::log("CALECITA: Dejo entrar a los chicos");

			for(vector<pid_t>::iterator it = kids.begin(); it != kids.end(); ++it){
				pid_t kidPid = (*it);
				stringstream ss;
				ss << KID_FIFO << kidPid;
				FifoEscritura chico(ss.str());
				chico.abrir();
				chico.escribir(static_cast<const void*> (&myPid), sizeof(pid_t));
				chico.cerrar();
			}
			kids.clear();

			Logger::log("CALECITA: Espero que entren todos los chicos");
			calecita.liberarLock();
			calecita.esperarEntradaChicos(cant_asientos);

			//calecita.tomarLock();
			Logger::log("CALECITA: Inicia la vuelta s: %d", t_vuelta);

			sleep(t_vuelta);

			Logger::log("CALECITA: fin vuelta");

			exitLock.liberarLock();
			calecita.liberarLock();
		}
	}

	fila.cerrar();
	fila.eliminar();

	Logger::log("CALECITA: END");
	return 0;
}
