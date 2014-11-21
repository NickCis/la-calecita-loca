#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SERVER_MSGTYP 1

typedef struct {
	char nombre[61];
	char direccion[120];
	char telefono[13];
} Register;

typedef enum {
	SELECT=0,
	INSERT,
	DELETE
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
