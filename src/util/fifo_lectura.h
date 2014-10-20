#ifndef FIFOLECTURA_H_
#define FIFOLECTURA_H_

#include "fifo.h"

class FifoLectura : public Fifo {
public:
	/** Crea una fifo de lectura
	 * @param nombre: path de archivo de la fifo
	 * @param erase: booleano que indica si se debe borrar el archivo de la fifo antes de crearlo (por si llegase a existir)
	 */
	FifoLectura(const std::string nombre);
	~FifoLectura();

	/** Abre la fifo (realiza open)
	 * @return 0 ok, resto mal
	 */
	int open();

	/** Lee datos de la fifo.
	 * @param buffer: destino de donde se pone la informacion leida (debe tener memoria asignada)
	 * @param buffsize: tama~no del buffer (y de lo que se quiere leer)
	 * @return tama~no que se leyo realmente.
	 */
	ssize_t read(void* buffer,const ssize_t buffsize) const;
};

#endif /* FIFOLECTURA_H_ */
