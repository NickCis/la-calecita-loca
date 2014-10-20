#include "lock_file.h"

using std::string;

LockFile::LockFile(const string &nombre){
	this->nombre = nombre;
	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	//this->fd = open ( this->nombre.c_str(),O_CREAT|O_WRONLY,0777 );
	this->fd = open(this->nombre.c_str(), O_CREAT | O_RDWR, 0777);

	if(this->fd == -1)
		throw string(__func__)+string(" :: error creando fd file: ");
}

int LockFile::tomarLock(bool write) {
	this->fl.l_type = write ? F_WRLCK : F_RDLCK;
	return fcntl(this->fd, F_SETLKW, &(this->fl));
}

int LockFile::liberarLock () {
	this->fl.l_type = F_UNLCK;
	return fcntl(this->fd, F_SETLK, &(this->fl));
}

ssize_t LockFile::escribir(const void* buffer, const ssize_t buffsize) const {
	lseek(this->fd, 0, SEEK_END);
	return write(this->fd,buffer, buffsize);
}

LockFile::~LockFile () {
	close(this->fd);
}
