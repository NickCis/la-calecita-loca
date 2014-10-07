#include "fifo.h"

Fifo::Fifo(const std::string nombre, bool erase) : nombre(nombre), fd(-1) {
	if(erase)
		unlink(nombre.c_str());
	mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 );
}

Fifo::~Fifo() {
}

void Fifo::cerrar() {
	close ( fd );
	fd = -1;
}

void Fifo::eliminar() const {
	unlink ( nombre.c_str() );
}
