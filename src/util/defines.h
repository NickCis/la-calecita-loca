#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <unistd.h>

#define SELLER_FIFO "/tmp/seller_fifo" // -> Path de archivo fifo que leera el vendedor
#define MONEY_BOX "/tmp/money.box" // -> Path de SHM para guardar la recaudacion
#define MONEY_BOX_CHAR 'A' // -> Letra de la SHM donde se guarda la recaudacion
#define QUEUE_FIFO "/tmp/queue_fifo" // -> Path de archivo fifo que leera la calesita (Cola que hacen los chicos para entrar)
#define KID_FIFO "/tmp/kid_fifo_" // -> Path de archivo fifo que leen los chicos
#define POSICIONES_CALESITA "/tmp/calesita.pos" // -> Path de SHM donde se guardan las posiciones de la calesita
#define POSICIONES_CALESITA_CHAR 'A' // -> Letra de la SHM donde se guardan las posiciones de la calesita
#define DENTRO_CALESITA "/tmp/calesita_dentro" // -> Path del semaforo donde escriben los chicos para decir que ya entraron y lee la calesita para saber que ya entraron los chicos

#define SALIDA_LOCK "/tmp/salida.lock" // -> Path de lock file que se usa para que los chicos salgan de la calesita
#define KIDS_OUT "/tmp/kids_out" // -> Path de lock file que se usa para que la calesita sepa cuando ya salieron todos los chivos

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

#endif
