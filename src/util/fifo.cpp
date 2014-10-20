#include "fifo.h"
#include <string>

using std::string;

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
}

Fifo::~Fifo() {
}

int Fifo::mknod() {
	return ::mknod(static_cast<const char*>(nombre.c_str()), S_IFIFO | 0666, 0);
}

int Fifo::close() {
	if(::close(fd))
		return -1;

	fd = -1;

	return 0;
}

int Fifo::unlink() const {
	return ::unlink(nombre.c_str());
}
