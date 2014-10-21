#include "../util/logger.h"
#include "../util/defines.h"
#include <unistd.h>
#include "../util/lock_file.h"
#include "../util/memoria_compartida.h"

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);
	Logger::log("Inicio");

	LockFile lock(MONEY_BOX);

	MemoriaCompartida<int> box(MONEY_BOX);
	lock.tomarLock();
	int recaudacion = box.leer();
	lock.liberarLock();

	Logger::log("Recaudacion %d", recaudacion);
	Logger::log("Salgo");
	return 0;
}
