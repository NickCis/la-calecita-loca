#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"
#include "../util/env_config.h"

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include <vector>

using std::vector;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("CALECITA");

	int cant_asientos = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS),
		t_vuelta = Config::getInt(ENVIROMENT_T_VUELTA, DEFAULT_TIEMPO_VUELTA);

	FifoLectura fila(QUEUE_FIFO);
	fila.abrir(true);
	ssize_t bytesLeidos;

	KidPosition kid;
	int cantidad = 0;

	vector<KidPosition> kids;

	for(;;){
		bytesLeidos = fila.leer(static_cast<void*> (&kid), sizeof(KidPosition));
		if(bytesLeidos != sizeof(KidPosition)){
			continue;
		}
		
		Logger::log("CALECITA: kid.pid %d kid.pos %d", kid.pid, kid.pos);

		kids.push_back(kid);

		if(++cantidad >= cant_asientos){
			cantidad = 0;
			Logger::log("CALECITA: iniciando vuelta");
			sleep(t_vuelta);
			Logger::log("CALECITA: fin vuelta");

			for(vector<KidPosition>::iterator it = kids.begin(); it != kids.end(); ++it){
				kill((*it).pid, SIGUSR1);
			}

			kids.clear();
		}
	}

	fila.cerrar();

	Logger::log("CALECITA: END");
	return 0;
}
