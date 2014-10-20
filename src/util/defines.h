#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <unistd.h>

#define SELLER_FIFO "/tmp/seller_fifo" // -> Path de archivo fifo que leera el vendedor
#define MONEY_BOX "/tmp/money.box" // -> Path de SHM para guardar la recaudacion
#define MONEY_BOX_CHAR 'A' // -> Letra de la SHM donde se guarda la recaudacion
#define QUEUE_CALESITA_FIFO "/tmp/queue_calesita_fifo" // -> Path de archivo fifo que leera la calesita (Cola que hacen los chicos para entrar)
#define QUEUE_CALESITA_SEM "/tmp/queue_calesita_sem"
#define KID_FIFO "/tmp/kid_fifo_" // -> Path de archivo fifo que leen los chicos
#define POSICIONES_CALESITA "/tmp/calesita.pos" // -> Path de SHM donde se guardan las posiciones de la calesita
#define POSICIONES_CALESITA_CHAR 'A' // -> Letra de la SHM donde se guardan las posiciones de la calesita

#define CALESITA_BIN "calesita"
#define SELLER_BIN "seller"
#define KID_BIN "kid"

#define SALIDA_LOCK "/tmp/salida.lock" // -> Path de lock file que se usa para controlar la salida a la calesita

#define DEFAULT_TICKET_COST 5 // -> Valor por defecto del precio de una entrada
#define DEFAULT_CANT_ASIENTOS 5 // -> Valor por defecto de la cantidad de asientos de la calesita
#define DEFAULT_CANT_CHICOS 8 // -> Valor por defecto de la cantidad de chicos en el barrio
#define DEFAULT_TIEMPO_VUELTA 2 // -> Valor por defecto del tiempo de una vuelta de la calesita
#define DEFAULT_LOG_FILE "./debug.log" // -> Valor por defecto del archivo de salida en modo DEBUG

#define ENVIROMENT_TICKET_COST "CALESITA_TICKET_COST" // -> Nombre de la variable de entorno para el precio de la calesita
#define ENVIROMENT_CANT_ASIENTOS "CALESITA_CANT_ASIENTOS" // -> Nombre de la variable de entorno para la cantidad de asientos de la calesita
#define ENVIROMENT_CANT_CHICOS "CALESITA_CANT_CHICOS" // -> Nombre de la variable de entorno para la cantidad de chicos en el barrio
#define ENVIROMENT_T_VUELTA "CALESITA_T_VUELTA" // -> Nombre de la variable de entorno para el tiempo de una vuelta en la calesita
#define ENVIROMENT_MODE "CALESITA_MODE" // -> Nombre de la variable de entorno para el modo de la calesita
#define ENVIROMENT_PATH "CALESITA_PATH" // -> Nombre de la variable de entorno para el archivo de salida en modo DEBUG


// No editable -----

#define EXIT_ON_TRUE_EXE(X, CODE) if(X){\
	Logger::log("%s:: sali por '" #X "' file: %s line: %d",  __func__, __FILE__, __LINE__);\
	CODE\
	return -1;\
}

#define EXIT_ON_TRUE(X) EXIT_ON_TRUE_EXE(X, ;)

#define CREATE_UNIQUE_PTR_EXE(NAME, TYPE, NEW, EXE)\
	unique_ptr<TYPE> NAME;\
	try {\
		NAME = unique_ptr<TYPE>(NEW);\
	}catch(string& str){\
		Logger::log("Exepcion: %s file: '%s' line: %d", str.c_str(), __FILE__, __LINE__);\
		EXE\
		return -1;\
	}catch(...){\
		Logger::log("Exepcion file: '%s' line: %d", __FILE__, __LINE__);\
		EXE\
		return -1;\
	}

#define CREATE_UNIQUE_PTR(NAME, TYPE, NEW) CREATE_UNIQUE_PTR_EXE(NAME, TYPE, NEW, ;)

enum class CalesitaSemaforos {
	dentroCalesita = 0,
	kidsOut,
	colaCalesita,
	totalSemaforos
};
#endif
