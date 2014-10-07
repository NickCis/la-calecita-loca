#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <unistd.h>

#define SELLER_FIFO "/tmp/seller_fifo"
#define MONEY_BOX "/tmp/money.box"
#define MONEY_BOX_CHAR 'A'
#define QUEUE_FIFO "/tmp/queue_fifo"
#define KID_FIFO "/tmp/kid_fifo_"
#define POSICIONES_CALECITA "/tmp/calecita.pos"
#define POSICIONES_CALECITA_CHAR 'A'
#define DENTRO_CALECITA_FIFO "/tmp/calecita_dentro"

#define SALIDA_LOCK "/tmp/salida.lock"

#define DEFAULT_TICKET_COST 5
#define DEFAULT_CANT_ASIENTOS 5
#define DEFAULT_CANT_CHICOS 8
#define DEFAULT_TIEMPO_VUELTA 2
#define DEFAULT_LOG_FILE "./debug.log"

#define ENVIROMENT_TICKET_COST "CALECITA_TICKET_COST"
#define ENVIROMENT_CANT_ASIENTOS "CALECITA_CANT_ASIENTOS"
#define ENVIROMENT_CANT_CHICOS "CALECITA_CANT_CHICOS"
#define ENVIROMENT_T_VUELTA "CALECITA_T_VUELTA"
#define ENVIROMENT_MODE "CALECITA_MODE"
#define ENVIROMENT_PATH "CALECITA_PATH"

#endif
