#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>

/** Clase para manejar File Locks
 */
class LockFile {

private:
	/** Estructura que describe la forma de lock.
	 */
	struct flock fl;

	/** File descriptor del archivo de lock
	 */
	int fd;

	/** Path del archivo
	 */
	std::string nombre;

public:
	/** Constructor de LockFile.
	 * @param nombre: path del archivo a usar de lock
	 */
	LockFile(const std::string &nombre);
	~LockFile();

	/** Toma el lock del archivo.
	 * Por defecto tomar el lock es realizado en modo escritura, si el parametro write es falso, el lock se tomara en modo escritura, la diferencia entre estos dos modos es que varios procesos pueden bloquear un lock en modo lectura, pero uno solo lo puede bloquear en modo escritura. Es decir, si muchos procesos bloquearon en modo lectura y otro proceso quiere bloquear en modo escritura, este ultimo tendra que esperar a todos los otros procesos para poder tomar el block.
	 * @param len: Largo a bloquear [defecto: 0]
	 * @param start: desde donde [defecto: 0]
	 * @param whence: desde donde se miden las cosas [defecto: inicio]
	 * @param type: read o write [defecto write]
	 * @return salida de fcntl
	 */
	int tomarLock(int len=0, int start=0, int whence=SEEK_SET, int type=F_WRLCK);

	/** Libera lock
	 */
	int liberarLock();

	/** Escribe en el archivo.
	 * @param buffer: buffer que se quiere escribir
	 * @param buffsize: tama~no del buffer que se quiere escribir
	 * @param offset offset
	 * @param whence desde donde se cuenta el offset
	 * @return tama~no que se escribio realmente
	 */
	ssize_t write(const void* buffer, const ssize_t buffsize, off_t offset=0, int whence=SEEK_END);

	/** Escribe en el archivo.
	 * @param buffer: buffer que se quiere escribir
	 * @param buffsize: tama~no del buffer que se quiere escribir
	 * @param offset offset
	 * @param whence desde donde se cuenta el offset
	 * @return tama~no que se escribio realmente
	 */
	ssize_t read(void* buffer, const ssize_t buffsize, off_t offset=0, int whence=SEEK_SET);
};

#endif /* LOCKFILE_H_ */
