#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SERVER_MSGTYP 1

#define FTOK_FILE "bin/server"
#define FTOK_CHAR 'a'

typedef struct {
	char nombre[61];
	char direccion[120];
	char telefono[13];
} Register;

typedef enum {
	SELECT=0,
	INSERT,
	DELETE,
	UPDATE,
	ERROR,
	SUCCESS
} MsgType;

typedef struct {
	MsgType type;
	pid_t pid;
	Register reg;
} MsgData;

typedef struct {
	long mtype;
	MsgData data;
} MsgStruct ;

#endif
