#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#include "../util/defines.h"

void usage(char *name){
	printf("Usage: %s [OPTION] NOMBRE [DIRECCION] [TEL]\n"
			"  -s, --select\t\tSelect, solo se debe especificar nombre\n"
			"  -i, --insert\t\tInsert, se debe especificar nombre direccion y telefono\n"
			"  -d, --delete\t\tDelete, solo se debe especificar nombre\n", name
		  );
}

int parse_cmd(int argc, char *argv[], MsgType *type, Register *reg){
	opterr = 0;
	int c;
	int required_args = 1;
	*type = SELECT;

	static struct option long_options[] = {
		{"select", no_argument, 0, 's'},
		{"insert", no_argument, 0, 'i'},
		{"delete", no_argument, 0, 'd'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	int option_index = 0;

	while((c = getopt_long (argc, argv, "hsid", long_options, &option_index))){
		if (c == -1)
			break;

		switch (c){
			case 'h':
				usage(argv[0]);
				return -1;
				break;

			case 's':
				required_args = 1;
				*type = SELECT;
				break;

			case 'i':
				required_args = 3;
				*type = INSERT;
				break;

			case 'd':
				required_args = 1;
				*type = DELETE;
				break;

			case '?':
			default:
				printf("bla\n");
				return -1;
				break;
		}
	}

	if(argc - optind < required_args){
		printf("Argumentos insuficientes\n");
		return -1;
	}

	switch(*type){
		case INSERT:
			strcpy(reg->nombre, argv[optind]);
			strcpy(reg->direccion, argv[optind+1]);
			strcpy(reg->telefono, argv[optind+2]);
			break;
		default:
			strcpy(reg->nombre, argv[optind]);
			break;
	}

	return 0;
}

int main(int argc, char *argv[]){
	MsgStruct msg;
	pid_t my_pid = getpid();
	memset((void*) &msg, 0, sizeof(MsgStruct));

	if(parse_cmd(argc, argv, &msg.data.type, &msg.data.reg))
		return -1;

	key_t key = ftok("bin/server", 'a');
	if(key == -1){
		printf("Error :: ftok(\"bin/server\", 'a') :: errno %d %s\n", errno, strerror(errno));
		return -1;
	}

	int msgid = msgget(key, 0666);
	if(msgid == -1){
		printf("Error :: msgget(key=%d, 0666) :: errno %d %s\n", key, errno, strerror(errno));
		return -1;
	}

	msg.mtype = SERVER_MSGTYP;
	msg.data.pid = my_pid;

	if(msgsnd(msgid, &msg, sizeof(MsgData), 0) == -1){
		printf("Error :: msgsnd(msgid=%d, &msg, sizeof(MsgData)=%d, 0) :: errno %d %s\n", msgid, sizeof(MsgData), errno, strerror(errno));
		return -1;
	}

	if(msg.data.type == SELECT){
		if(msgrcv(msgid, &msg, sizeof(MsgData), my_pid, 0) == -1){
			printf("Error :: msgrcv(msgid=%d, &msg, sizeof(MsgData)=%d, my_pid=%d, 0) :: errno %d %s\n", msgid, sizeof(MsgData), my_pid, errno, strerror(errno));
			return -1;
		}

		printf("Nombre: '%s'\nDireccion: '%s'\nTelefono: '%s'\n", msg.data.reg.nombre, msg.data.reg.direccion, msg.data.reg.telefono);
	}

	return 0;

}
