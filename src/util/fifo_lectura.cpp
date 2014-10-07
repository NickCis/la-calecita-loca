#include "fifo_lectura.h"

FifoLectura::FifoLectura(const std::string nombre, bool erase) : Fifo(nombre, erase) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	this->abrir(false);
}
void FifoLectura::abrir(bool rw) {
	fd = open ( nombre.c_str(), rw ? O_RDWR : O_RDONLY );
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	return read ( fd,buffer,buffsize );
}
