#ifndef FIFOESCRITURA_H_
#define FIFOESCRITURA_H_

#include "fifo.h"

class FifoEscritura : public Fifo {
public:
	/** Crea una fifo de escritura
	 * @param nombre: path de archivo de la fifo
	 * @param erase: booleano que indica si se debe borrar el archivo de la fifo antes de crearlo (por si llegase a existir)
	 */
	FifoEscritura(const std::string nombre);
	~FifoEscritura();

	/** Abre la fifo (realiza open)
	 * @return 0 bien, resto mal
	 */
	int open();

	/** Escribe datos de la fifo.
	 * @param buffer: datos que se escribiran
	 * @param buffsize: tama~no del buffer
	 * @return tama~no de lo que se escribio realmente
	 */
	ssize_t write(const void* buffer, const ssize_t buffsize) const;
};

#endif /* FIFOESCRITURA_H_ */
