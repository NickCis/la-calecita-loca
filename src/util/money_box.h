#include "../util/memoria_compartida.h"
#include "../util/lock_file.h"

#include <memory>

class MoneyBox {
	protected:
		std::unique_ptr<MemoriaCompartida<int>> money;
		std::unique_ptr<LockFile> lock;

	public:
		MoneyBox();
		~MoneyBox();

		/** Devuelve valor de la recaudacion
		 * @return valor de la recaudacion
		 */
		int read();

		/** Escribe nuevo valor en la recaudacion
		 * @param val nuevo valor de la recaudacion
		 */
		void write(int val);

		/** Borra lock file.
		 * Ejecuta LockFile::unlink
		 * @return 0 ok, resto error
		 */
		int unlink();
};
