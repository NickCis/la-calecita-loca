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
	 * @param erase: booleano que indica si se debe borrar el archivo de la fifo antes de crearlo (por si llegase a existir)
	 */
	Fifo(const std::string nombre, bool erase = false);
	virtual ~Fifo();

	/** Abre la fifo (realiza open)
	 */
	virtual void abrir() = 0;

	/** Cierra la fifo (realiza close)
	 */
	void cerrar();

	/** Elimina la fifo, (realiza unlink)
	 */
	void eliminar() const;

protected:
	/** Path de la fifo
	 */
	std::string nombre;

	/** file descriptor de la fifo
	 */
	int fd;
};

#endif /* FIFO_H_ */
