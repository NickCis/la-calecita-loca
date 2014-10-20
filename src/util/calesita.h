#ifndef __CALESITA_H__
#define __CALESITA_H__

#include <unistd.h>

#include "lock_file.h"
#include "semaforo.h"
#include "memoria_compartida3.h"

#include <memory>

class Calesita {
	protected:
		/** Tama~no de la memoria compartida
		 */
		size_t size;

		/** Cantidad de posiciones en la calesita
		 */
		int cantidadPosiciones;

		/** Lock para la shm
		 */
		LockFile posicionesLock;

		/** FileLock usado para trabar la salida de la calesita
		 */
		LockFile exitLock;

		/** Semaforo usado para ver cuando entraron todos los chicos a la calesita
		 */
		std::unique_ptr<Semaforo> dentroCalesita;

		/** Semaforo usado para trabar el inicio de la siguiente vuelta mientras siga habiendo chicos en la calesita
		 */
		std::unique_ptr<Semaforo> kidsOut;

		Calesita();
		~Calesita();
};

class CalesitaUsuario: public Calesita {
	protected:
		/** Bloquea posicion pos.
		 * @param pos posicion a bloquear
		 * @return 0 ok, resto error
		 */
		int lockPosicion(int pos);

		/** Lee posicion pos.
		 * @param pos posicion a leer
		 * @return valor de la posicion (o negativo si error)
		 */
		pid_t leerPosicion(int pos);

		/** Escribe posicion en pos.
		 * @param pos posicion a escribri
		 * @return 0 ok, resto error
		 */
		int escribirPosicion(int pos);
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
