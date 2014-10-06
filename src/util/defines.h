#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <unistd.h>

#define SELLER_FIFO "/tmp/seller_fifo"
#define MONEY_BOX "money.box"
#define MONEY_BOX_CHAR 'A'
#define QUEUE_FIFO "/tmp/queue_fifo"

typedef struct {
	pid_t pid;
	int pos;
} KidPosition;

#endif
