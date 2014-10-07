#ifndef __CALECITA_H__
#define __CALECITA_H__

#include <unistd.h>

#include "lock_file.h"
#include "memoria_compartida3.h"

class Calecita {
	private:
		MemoriaCompartida3<pid_t> *shm;
		size_t size;
		int cantidadPosiciones;
		LockFile lock;
		pid_t *posiciones;

	public:
		Calecita();
		~Calecita();

		/** Toma lock del lock de archivo de la shm asignada a la memoria compartida de la calecita
		 * @return salida de LockFile::tomarLock()
		 */
		int tomarLock();

		/** Libera lock del lock de archivo de la shm asignada a la memoria compartida de la calecita
		 * @return salida de LockFile::liberarLock()
		 */
		int liberarLock();

		/** Ocupa la posicion deseada, de estar ocupada ocupa otra cercana.
		 * No toma lock, hace falta hacerlo externamente
		 * @param pos posicion a ocupar
		 * @return posicion ocupada realmente o -1 si error
		 */
		int ocuparPosicion(int pos);

		void clear();

		/** Esperar a que entren todos los chicos.
		 * @param cantChicos cantidad de chicos que hay que esperar que entren
		 */
		void esperarEntradaChicos(int cantChicos);
};
#endif
