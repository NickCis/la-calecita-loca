#include "../util/defines.h"
#include "../util/env_config.h"

#include <iostream>
#include <string>

#include <sys/wait.h>

using std::cout;
using std::endl;
using std::string;
using std::cin;

int runExe(string exe, int &exit){
	int pid = fork();
	if(pid == 0){ // Child
		exit = execl(exe.c_str(), exe.c_str(), NULL);
	}

	return pid;
}

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
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


	string binPath = Config::getBinPath();
	int exit = 0;

#define EXEC_PROGRAM(X) cout << "Ejecutando " X << endl;\
	if(! runExe(binPath+"/" X, exit))\
		return exit

	EXEC_PROGRAM(SELLER_BIN);
	EXEC_PROGRAM(CALESITA_BIN);

	sleep(5);

	for(int i=0; i < chicos ; i++){
		EXEC_PROGRAM("kid");
	}

	cout << "Espero a que mueran todos los programas" << endl;
	while(wait(NULL) > 0);

	cout << "Fin simulacion" << endl;

	return 0;

}
