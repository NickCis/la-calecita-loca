#include "fifo_escritura.h"

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre) {
}

FifoEscritura::~FifoEscritura() {
}

int FifoEscritura::open() {
	return !(fd = ::open(nombre.c_str(), O_WRONLY));
}

ssize_t FifoEscritura::write(const void* buffer,const ssize_t buffsize) const {
	return ::write(fd, buffer, buffsize);
}
