#include "fifo.h"
#include <string>

using std::string;

Fifo::Fifo(const std::string nombre, bool erase) : nombre(nombre), fd(-1) {
	if(erase)
		unlink(nombre.c_str());
	if(mknod(static_cast<const char*>(nombre.c_str()), S_IFIFO | 0666, 0)){
		//throw string("Error con mknod");
	}
}

Fifo::~Fifo() {
}

int Fifo::cerrar() {
	if(close(fd))
		return -1;

	fd = -1;

	return 0;
}

int Fifo::eliminar() const {
	return unlink(nombre.c_str());
}
