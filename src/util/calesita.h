#ifndef __CALESITA_H__
#define __CALESITA_H__

#include <unistd.h>

#include "lock_file.h"
#include "memoria_compartida3.h"

class Calesita {
	private:
		/** Memoria compartida usada para las posiciones de la calesita
		 */
		MemoriaCompartida3<pid_t> *shm;

		/** Tama~no de la memoria compartida
		 */
		size_t size;

		/** Cantidad de posiciones en la calesita
		 */
		int cantidadPosiciones;

		/** Lock para la shm
		 */
		LockFile lock;

		/** Posiciones de la calesita (Leidas de la shm)
		 */
		pid_t *posiciones;

	public:
		Calesita();
		~Calesita();

		/** Toma lock del lock de archivo de la shm asignada a la memoria compartida de la calesita
		 * @return salida de LockFile::tomarLock()
		 */
		int tomarLock();

		/** Libera lock del lock de archivo de la shm asignada a la memoria compartida de la calesita
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
