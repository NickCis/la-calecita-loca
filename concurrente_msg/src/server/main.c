#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../util/defines.h"

typedef struct {
	char valid;
	Register reg;
} FileRegister;

static sig_atomic_t running = 1;
static int fd = 0;
static const char DATA_PATH[] = "/tmp/db.data";

/** Abre el archivo
 * @return -1 error, 0 ok
 */
int open_file();

/** Cierra el archivo
 */
void close_file();

/** Inserta un registro en la base de datos.
 * De ya existir el registro devuelve error.
 * @param reg [in] registro a insertar
 * @return == -1 error, 0 ok, 1 ya existe
 */
int insert_reg(const Register *reg);

/** Actualiza un registro de la base de datos
 * @param reg [in] se usa el name para buscar, y el resto de los datos para actualizar
 * @return == -1 error, 0 ok, 1 no encontro
 */
int update_reg(const Register *reg);

/** Borra un registro de la base de datos
 * @param reg [in] se usa el name para buscar
 * @return == -1 error, 0 ok, 1 no encontro
 */
int delete_reg(const Register *reg);

/** Busca un registro en la base de datos.
 * @param reg [in/out] se usa el name para buscar, se devuelve por este registro
 * @return == -1 error, 0 ok, 1 no encontro
 */
int select_reg(Register *reg);

int open_file(){
	if(fd)
		return 0;

	fd = open(DATA_PATH, O_CREAT | O_RDWR, 0666);
	if(fd == -1){
		printf("Error :: %s :: open(DATA_PATH='%s', O_CREAT | O_RDWR, 0666) :: errno %d '%s'\n", __func__, DATA_PATH, errno, strerror(errno));
		return -1;
	}

	return 0;
}

void close_file(){
	if(fd <= 0)
		return;

	if(close(fd) == -1)
		printf("Error :: %s :: close(fd=%d) :: errno %d '%s'\n", __func__, fd, errno, strerror(errno));
}

int insert_reg(const Register *reg){
	int ret;

	FileRegister file_reg;
	memcpy((void*) &file_reg.reg, (void*) reg, sizeof(Register));

	if((ret = select_reg(&file_reg.reg)) <= 0)
		return ret == 0 ? 1 : ret;

	file_reg.valid = 1;
	if(write(fd, &file_reg, sizeof(FileRegister)) != sizeof(FileRegister)){
		printf("Error :: %s :: write(fd=%d, &file_reg=%p, sizeof(FileRegister)=%d) != sizeof(FileRegister) :: errno %d '%s'\n", __func__, fd, &file_reg, sizeof(FileRegister), errno, strerror(errno));
		return -1;
	}

	return 0;
}

int update_reg(const Register *reg){
	int ret;

	FileRegister file_reg;
	memcpy((void*) &file_reg.reg, (void*) reg, sizeof(Register));

	if((ret = select_reg(&file_reg.reg)))
		return ret;

	if(lseek(fd, -sizeof(FileRegister), SEEK_CUR) == -1){
		printf("Error :: %s :: lseek(fd=%d, -sizeof(FileRegister)=%d, SEEK_CUR) :: errno %d '%s'\n", __func__, fd, -sizeof(FileRegister), errno, strerror(errno));
		return -1;
	}

	memcpy((void*) &file_reg.reg, (void*) reg, sizeof(Register));
	file_reg.valid = 1;

	if(write(fd, &file_reg, sizeof(FileRegister)) != sizeof(FileRegister)){
		printf("Error :: %s :: write(fd=%d, &file_reg=%p, sizeof(FileRegister)=%d) != sizeof(FileRegister) :: errno %d '%s'\n", __func__, fd, &file_reg, sizeof(FileRegister), errno, strerror(errno));
		return -1;
	}

	return 0;
}

int delete_reg(const Register *reg){
	int ret;

	FileRegister file_reg;
	memcpy((void*) &file_reg.reg, (void*) reg, sizeof(Register));

	if((ret = select_reg(&file_reg.reg)))
		return ret;

	if(lseek(fd, -sizeof(FileRegister), SEEK_CUR) == -1){
		printf("Error :: %s :: lseek(fd=%d, -sizeof(FileRegister)=%d, SEEK_CUR) :: errno %d '%s'\n", __func__, fd, -sizeof(FileRegister), errno, strerror(errno));
		return -1;
	}

	file_reg.valid = 0;

	if(write(fd, &file_reg, sizeof(FileRegister)) != sizeof(FileRegister)){
		printf("Error :: %s :: write(fd=%d, &file_reg=%p, sizeof(FileRegister)=%d) != sizeof(FileRegister) :: errno %d '%s'\n", __func__, fd, &file_reg, sizeof(FileRegister), errno, strerror(errno));
		return -1;
	}

	return 0;
}

int select_reg(Register *reg){
	int ret;
	if(!fd)
		if((ret = open_file()))
			return ret;

	if(lseek(fd, 0, SEEK_SET) == -1){
		printf("Error :: %s :: lseek(fd=%d, 0, SEEK_SET) :: errno %d '%s'\n", __func__, fd, errno, strerror(errno));
		return -1;
	}

	FileRegister file_reg;
	while((read(fd, &file_reg, sizeof(FileRegister)) == sizeof(FileRegister))){
		if(file_reg.valid && strcmp(file_reg.reg.nombre, reg->nombre) == 0){
			memcpy((void*) reg, (void*) &file_reg.reg, sizeof(Register));
			return 0;
		}
	}

	return 1;
}

void end_program(int sig __attribute__ ((unused))){
	running = 0;
}

int main(int argc __attribute__ ((unused)), char *argv[] __attribute__ ((unused))){
	key_t key = ftok(FTOK_FILE, FTOK_CHAR);
	if(key == -1){
		printf("Error :: %s :: ftok(\"%s\", '%c') :: errno %d %s\n", __func__, FTOK_FILE, FTOK_CHAR, errno, strerror(errno));
		return -1;
	}

	int msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
	if(msgid == -1){
		printf("Error :: %s :: msgget(key=%d, IPC_CREAT | IPC_EXCL | 0666) :: errno %d %s\n", __func__, key, errno, strerror(errno));
		return -1;
	}

	// Registro se~nales para cerrar de manera linda
	signal(SIGHUP, end_program);
	signal(SIGQUIT, end_program);
	signal(SIGKILL, end_program);
	signal(SIGINT, end_program);

	MsgStruct msg;
	int ret;

	printf("Server running msgid: %d\n", msgid);
	while(running && msgrcv(msgid, &msg, sizeof(MsgData), SERVER_MSGTYP, 0) != -1){
		switch(msg.data.type){
			case INSERT:
				printf("[INFO] :: INSERT (%d) :  pid: %d nombre: '%s' direcccion: '%s' telefono: '%s'\n", msg.data.type, msg.data.pid, msg.data.reg.nombre, msg.data.reg.direccion, msg.data.reg.telefono);
				msg.data.type = SUCCESS;
				if((ret = insert_reg(&msg.data.reg))){
					msg.data.type = ERROR;
					printf("Error :: %s :: insert_reg() -> %d\n", __func__, ret);
				}
				break;

			case SELECT:
				printf("[INFO] :: SELECT (%d) :  pid: %d nombre: '%s'\n", msg.data.type, msg.data.pid, msg.data.reg.nombre);
				if((ret = select_reg(&msg.data.reg))){
					msg.data.type = ERROR;
					printf("Error :: %s :: select_reg() -> %d\n", __func__, ret);
				}
				break;

			case UPDATE:
				printf("[INFO] :: UPDATE (%d) :  pid: %d nombre: '%s' direcccion: '%s' telefono: '%s'\n", msg.data.type, msg.data.pid, msg.data.reg.nombre, msg.data.reg.direccion, msg.data.reg.telefono);
				msg.data.type = SUCCESS;
				if((ret = update_reg(&msg.data.reg))){
					msg.data.type = ERROR;
					printf("Error :: %s :: update_reg() -> %d\n", __func__, ret);
				}
				break;

			case DELETE:
				printf("[INFO] :: DELETE (%d) :  pid: %d nombre: '%s'\n", msg.data.type, msg.data.pid, msg.data.reg.nombre);
				msg.data.type = SUCCESS;
				if((ret = delete_reg(&msg.data.reg))){
					msg.data.type = ERROR;
					printf("Error :: %s :: delete_reg() -> %d\n", __func__, ret);
				}
				break;

			default:
				break;
		}

		msg.mtype = msg.data.pid;
		if(msgsnd(msgid, &msg, sizeof(MsgData), 0) == -1)
			printf("Error :: %s :: msgsnd(msgid=%d, &msg, sizeof(MsgData)=%d, 0) :: errno %d '%s'\n", __func__, msgid, sizeof(MsgData), errno, strerror(errno));
	}

	if(msgctl(msgid, IPC_RMID, NULL) == -1){
		printf("Error :: %s :: msgctl(msgid=%d, IPC_RMID, NULL) :: errno %d '%s'\n", __func__, msgid, errno, strerror(errno));
	}

	close_file();

	return 0;
}
