#include "lock_file.h"

using std::string;

LockFile::LockFile(const string &nombre){
	this->nombre = nombre;

	//this->fd = open ( this->nombre.c_str(),O_CREAT|O_WRONLY,0777 );
	this->fd = open(this->nombre.c_str(), O_CREAT | O_RDWR, 0777);

	if(this->fd == -1)
		throw string(__func__)+string(" :: error creando fd file: ");
}

int LockFile::tomarLock(int len, int start, int whence, int type) {
	this->fl.l_type = type;
	this->fl.l_whence = whence;
	this->fl.l_start = start;
	this->fl.l_len = len;
	return fcntl(this->fd, F_SETLKW, &(this->fl));
}

int LockFile::liberarLock () {
	this->fl.l_type = F_UNLCK;
	return ::fcntl(this->fd, F_SETLK, &(this->fl));
}

ssize_t LockFile::write(const void* buffer, const ssize_t buffsize, off_t offset, int whence){
	lseek(this->fd, offset, whence);
	return ::write(this->fd,buffer, buffsize);
}

LockFile::~LockFile(){
	::close(this->fd);
}

ssize_t LockFile::read(void* buffer, const ssize_t buffsize, off_t offset, int whence){
	lseek(this->fd, offset, whence);
	return ::read(this->fd, buffer, buffsize);
}
