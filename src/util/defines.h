#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <unistd.h>

#define SELLER_FIFO "/tmp/seller_fifo"
#define MONEY_BOX "money.box"
#define MONEY_BOX_CHAR 'A'
#define QUEUE_FIFO "/tmp/queue_fifo"

#define DEFAULT_TICKET_COST 5
#define DEFAULT_CANT_ASIENTOS 5
#define DEFAULT_TIEMPO_VUELTA 2
#define DEFAULT_LOG_FILE "./debug.log"

#define ENVIROMENT_TICKET_COST "CALECITA_TICKET_COST"
#define ENVIROMENT_CANT_ASIENTOS "CALECITA_CANT_ASIENTOS"
#define ENVIROMENT_T_VUELTA "CALECITA_T_VUELTA"
#define ENVIROMENT_MODE "CALECITA_MODE"
#define ENVIROMENT_PATH "CALECITA_PATH"

typedef struct {
	pid_t pid;
	int pos;
} KidPosition;

#endif
