#include "../util/logger.h"
#include "../util/fifo_lectura.h"
#include "../util/defines.h"

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include <vector>

#define CANT_ASIENTOS 5
#define TIEMPO_VUELTA 2

using std::vector;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("CALECITA");

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

		if(++cantidad >= CANT_ASIENTOS){
			cantidad = 0;
			Logger::log("CALECITA: iniciando vuelta");
			sleep(2);
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
