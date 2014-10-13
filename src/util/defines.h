#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <unistd.h>

#define SELLER_FIFO "/tmp/seller_fifo"
#define MONEY_BOX "/tmp/money.box"
#define MONEY_BOX_CHAR 'A'
#define QUEUE_FIFO "/tmp/queue_fifo"
#define KID_FIFO "/tmp/kid_fifo_"
#define POSICIONES_CALESITA "/tmp/calesita.pos"
#define POSICIONES_CALESITA_CHAR 'A'
#define DENTRO_CALESITA_FIFO "/tmp/calesita_dentro"

#define SALIDA_LOCK "/tmp/salida.lock"
#define KIDS_OUT_LOCK "/tmp/kids_out.lock"

#define DEFAULT_TICKET_COST 5
#define DEFAULT_CANT_ASIENTOS 5
#define DEFAULT_CANT_CHICOS 8
#define DEFAULT_TIEMPO_VUELTA 2
#define DEFAULT_LOG_FILE "./debug.log"

#define ENVIROMENT_TICKET_COST "CALESITA_TICKET_COST"
#define ENVIROMENT_CANT_ASIENTOS "CALESITA_CANT_ASIENTOS"
#define ENVIROMENT_CANT_CHICOS "CALESITA_CANT_CHICOS"
#define ENVIROMENT_T_VUELTA "CALESITA_T_VUELTA"
#define ENVIROMENT_MODE "CALESITA_MODE"
#define ENVIROMENT_PATH "CALESITA_PATH"

#endif
