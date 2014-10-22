#include "../util/logger.h"
#include "../util/defines.h"
#include "../util/money_box.h"

#include <unistd.h>

#include <memory>
#include <string>

using std::string;
using std::unique_ptr;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);
	Logger::log("Inicio");

	CREATE_UNIQUE_PTR(box, MoneyBox, new MoneyBox());
	int recaudacion = box->read();

	Logger::log("Recaudacion %d", recaudacion);
	Logger::log("Salgo");
	return 0;
}
