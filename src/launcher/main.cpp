#include "../util/defines.h"
#include "../util/env_config.h"
#include "../util/logger.h"
#include "../util/money_box.h"
#include "../util/semaforo.h"

#include <iostream>
#include <string>
#include <memory>

#include <sys/wait.h>

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::unique_ptr;

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::setName(argv[0]);
	cout << "Lanzador de simulacion de calesita" << endl;
	string mode = Config::getStr(ENVIROMENT_MODE, "NORMAL"),
		   path = Config::getStr(ENVIROMENT_PATH, DEFAULT_LOG_FILE);
	int ticketCost = Config::getInt(ENVIROMENT_TICKET_COST, DEFAULT_TICKET_COST),
		asientos = Config::getInt(ENVIROMENT_CANT_ASIENTOS, DEFAULT_CANT_ASIENTOS),
		chicos = Config::getInt(ENVIROMENT_CANT_CHICOS, DEFAULT_CANT_CHICOS),
		tVuelta = Config::getInt(ENVIROMENT_T_VUELTA, DEFAULT_TIEMPO_VUELTA);

	cout << "Configuracion seteada:" << endl;
#define PRINT_VAR(X, Y) cout << "\t" X ": " << Y << endl;
	PRINT_VAR(ENVIROMENT_MODE, mode);
	PRINT_VAR(ENVIROMENT_PATH, path);
	PRINT_VAR(ENVIROMENT_TICKET_COST, ticketCost);
	PRINT_VAR(ENVIROMENT_CANT_ASIENTOS, asientos);
	PRINT_VAR(ENVIROMENT_CANT_CHICOS, chicos);
	PRINT_VAR(ENVIROMENT_T_VUELTA, tVuelta);
#undef PRINT_VAR

	cout << "Desea iniciar simulacion (s/n)" << endl;
	char c;
	cin >> c;
	if(c == 'n')
		return 0;

	Logger::log("Inicio de simulacion");
	Logger::log("Configuracion:");
	Logger::log("* %s=%s", ENVIROMENT_MODE, mode.c_str());
	Logger::log("* %s=%s", ENVIROMENT_PATH, path.c_str());
	Logger::log("* %s=%d", ENVIROMENT_TICKET_COST, ticketCost);
	Logger::log("* %s=%d", ENVIROMENT_CANT_ASIENTOS, asientos);
	Logger::log("* %s=%d", ENVIROMENT_CANT_CHICOS, chicos);
	Logger::log("* %s=%d", ENVIROMENT_T_VUELTA, tVuelta);

	CREATE_UNIQUE_PTR(semInit, Semaforo, new Semaforo(Config::getBinPath(LAUNCHER_BIN)));
	semInit->setVal(2);

	CREATE_UNIQUE_PTR(box, MoneyBox, new MoneyBox());

	string binPath = Config::getBinPath();

	#define EXEC_PROGRAM(X)\
		Logger::log("Ejecutando %s",  X);\
		if(fork() == 0){\
			string exe = binPath+"/" X;\
			return execl(exe.c_str(), exe.c_str(), NULL);\
		}

	EXEC_PROGRAM(SELLER_BIN);
	EXEC_PROGRAM(CALESITA_BIN);

	semInit->wait();

	Logger::log("Ya inicializaron %s y %s. Empiezo a lanzar %s", SELLER_BIN, CALESITA_BIN, KID_BIN);

	for(int i=0; i < chicos ; i++){
		EXEC_PROGRAM(KID_BIN);
	}

	Logger::log("Espero a que mueran todos los programas");

	while(wait(NULL) > 0);

	box->unlink();
	semInit->eliminar();

	Logger::log("Fin simulacion");

	return 0;
}
