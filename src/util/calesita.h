#ifndef __CALESITA_H__
#define __CALESITA_H__

#include <unistd.h>

#include "lock_file.h"
#include "semaforo.h"
#include "memoria_compartida3.h"

class Calesita {
	protected:
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

		/** Toma lock del lock de archivo de la shm asignada a la memoria compartida de la calesita
		 * @return salida de LockFile::tomarLock()
		 */
		int tomarLock();

		/** Libera lock del lock de archivo de la shm asignada a la memoria compartida de la calesita
		 * @return salida de LockFile::liberarLock()
		 */
		int liberarLock();

		Semaforo dentroCalesita;

		/** FileLock usado para trabar la salida de los chicos de la calesita
		 */
		LockFile exitLock;

		/** Semaforo usado para trabar el inicio de la siguiente vuelta mientras siga habiendo chicos en la calesita
		 */
		Semaforo kidsOut;

		Calesita();
		~Calesita();
};

class CalesitaUsuario: public Calesita {
	public:
		CalesitaUsuario();
		~CalesitaUsuario();

		/** Ocupa la posicion deseada, de estar ocupada ocupa otra cercana.
		 * @param pos posicion a ocupar
		 * @return posicion ocupada realmente o -1 si error
		 */
		int ocuparPosicion(int pos);

		/** Metodo que hace que el chico se diverta en la calesita, es decir, se queda esperando a que termine
		 * @return salida de los locks
		 */
		int divertirme();
};

class CalesitaControlador: public Calesita{
	protected:
		/** Limpia la shm de la calesita.
		 * @return 0 bien, resto error
		 */
		int clear();

	public:
		CalesitaControlador();
		~CalesitaControlador();

		/** Inicializa la calesita para una nueva vuelta.
		 * @param cantChicos cantidad de chicos que hay que esperar que entren
		 * @return 0 bien, resto error
		 */
		int inicializarNuevaVuelta(int cantChicos);

		/** Esperar a que entren todos los chicos.
		 * @return 0 ok, resto error
		 */
		int esperarEntradaChicos();

		/** Da una vuelta.
		 * @return 0 bien, resto error
		 */
		int darVuelta();

		/** Esperar a salida de todos los chicos
		 * @return 0 ok, resto error
		 */
		int esperarSalidaChicos();
};
#endif
