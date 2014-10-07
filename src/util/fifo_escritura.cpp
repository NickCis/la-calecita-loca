#include "fifo_escritura.h"

FifoEscritura::FifoEscritura(const std::string nombre, bool erase) : Fifo(nombre, erase) {
}

FifoEscritura::~FifoEscritura() {
}

void FifoEscritura::abrir() {
	fd = open ( nombre.c_str(),O_WRONLY );
}

ssize_t FifoEscritura::escribir(const void* buffer,const ssize_t buffsize) const {
	return write ( fd,buffer,buffsize );
}
