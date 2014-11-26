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
			"  -m, --msgid=MSGID\t\tEspecificar el msgid a usar, si no se generara.\n"
			"  -s, --select\t\tSelect, solo se debe especificar nombre\n"
			"  -d, --delete\t\tDelete, solo se debe especificar nombre\n"
			"  -i, --insert\t\tInsert, se debe especificar nombre direccion y telefono\n"
			"  -u, --update\t\tUpdate, se debe especificar nombre direccion y telefono\n", name
		  );
}

int parse_cmd(int argc, char *argv[], MsgType *type, Register *reg, int *msgid){
	opterr = 0;
	int c;
	int required_args = 1;
	*type = SELECT;

	static struct option long_options[] = {
		{"select", no_argument, 0, 's'},
		{"insert", no_argument, 0, 'i'},
		{"delete", no_argument, 0, 'd'},
		{"update", no_argument, 0, 'u'},
		{"msgid", required_argument, 0, 'm'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	int option_index = 0;

	while((c = getopt_long (argc, argv, "hsidum:", long_options, &option_index))){
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

			case 'u':
				required_args = 3;
				*type = UPDATE;
				break;

			case 'm':
				*msgid = atoi(optarg);
				break;

			case '?':
			default:
				printf("Argumento desconocido '%s'\n", optarg);
				return -1;
				break;
		}
	}

	if(argc - optind < required_args){
		printf("Argumentos insuficientes\n");
		return -1;
	}

	#define CONTROL_AND_COPY(X)\
	if(sizeof(reg->X) <= strlen(argv[optind])){\
		printf("Error :: \"" #X "\" mas largo que lo permitido. Max: %d, Largo: %d\n", sizeof(reg->X) -1, strlen(argv[optind]));\
		return -1;\
	}\
	strcpy(reg->X, argv[optind++])

	CONTROL_AND_COPY(nombre);

	switch(*type){
		case UPDATE:
		case INSERT:
			CONTROL_AND_COPY(direccion);
			CONTROL_AND_COPY(telefono);
			break;
		default:

			break;
	}

	#undef CONTROL_AND_COPY

	return 0;
}

int main(int argc, char *argv[]){
	MsgStruct msg;
	pid_t my_pid = getpid();
	memset((void*) &msg, 0, sizeof(MsgStruct));
	int msgid = 0;

	if(parse_cmd(argc, argv, &msg.data.type, &msg.data.reg, &msgid))
		return -1;

	if(msgid == 0){
		key_t key = ftok(FTOK_FILE, FTOK_CHAR);
		if(key == -1){
			printf("Error :: %s :: ftok(\"%s\", '%c') :: errno %d %s\n", __func__, FTOK_FILE, FTOK_CHAR, errno, strerror(errno));
			return -1;
		}

		msgid = msgget(key, 0666);
		if(msgid == -1){
			printf("Error :: %s :: msgget(key=%d, 0666) :: errno %d %s\n", __func__, key, errno, strerror(errno));
			return -1;
		}
	}

	msg.mtype = SERVER_MSGTYP;
	msg.data.pid = my_pid;

	if(msgsnd(msgid, &msg, sizeof(MsgData), 0) == -1){
		printf("Error :: %s :: msgsnd(msgid=%d, &msg, sizeof(MsgData)=%d, 0) :: errno %d %s\n", __func__, msgid, sizeof(MsgData), errno, strerror(errno));
		return -1;
	}

	if(msgrcv(msgid, &msg, sizeof(MsgData), my_pid, 0) == -1){
		printf("Error :: %s :: msgrcv(msgid=%d, &msg, sizeof(MsgData)=%d, my_pid=%d, 0) :: errno %d %s\n", __func__, msgid, sizeof(MsgData), my_pid, errno, strerror(errno));
		return -1;
	}

	switch(msg.data.type){
		case SELECT:
			printf("Nombre: '%s'\nDireccion: '%s'\nTelefono: '%s'\n", msg.data.reg.nombre, msg.data.reg.direccion, msg.data.reg.telefono);
			break;

		case SUCCESS:
			printf("Success\n");
			break;

		case ERROR:
		default:
			printf("Error\n");
			return -1;
			break;
	}

	return 0;
}
