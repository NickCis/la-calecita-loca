#ifndef FIFO_H_
#define FIFO_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/** Clase para utilizar una FIFO
 */
class Fifo {
public:
	/** Crea una fifo
	 * @param nombre: path de archivo de la fifo
	 */
	Fifo(const std::string nombre);
	virtual ~Fifo();

	/** Abre la fifo (realiza open)
	 * @return 0 bien, resto error
	 */
	virtual int open() = 0;

	/** Cierra la fifo (realiza close)
	 * @return 0 bien, resto error
	 */
	int close();

	/** Mknod del archivo
	 * @return 0 bien, resto error
	 */
	int mknod();

	/** Elimina la fifo, (realiza unlink)
	 * @return 0 bien, resto error
	 */
	int unlink() const;

protected:
	/** Path de la fifo
	 */
	std::string nombre;

	/** file descriptor de la fifo
	 */
	int fd;
};

#endif /* FIFO_H_ */
