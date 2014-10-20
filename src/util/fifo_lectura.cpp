#include "fifo_lectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

int FifoLectura::open() {
	return (fd = ::open(nombre.c_str(), O_RDONLY)) == -1;
}

ssize_t FifoLectura::read(void* buffer, const ssize_t buffsize) const {
	return ::read(fd, buffer, buffsize);
}
