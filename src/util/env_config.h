#ifndef __ENV_CONFIG_H__
#define __ENV_CONFIG_H__

#include <string>

namespace Config {

	/** Lee valor de las variables de entorno.
	 * Busca valor en las variables de entorno, y lo parsea al tipo pedido, de no encontrarlo deveulve value.
	 * @param name: nombre de la variable de entorno
	 * @param value: valor usado por defecto si no se encuentra
	 * @return valor de la variable o el por defecto
	 */
	int getInt(const std::string& name, const int& value);
	std::string getStr(const std::string& name, const std::string& value);

	/** Obtiene el path donde estan los ejecutables
	 * @return string donde esta el ejecutable
	 */
	std::string getBinPath();
	/** Obtiene el path donde estan los ejecutables, anteponiendole '/name/'
	 * @param name texto que se anteponpdra
	 * @return string donde esta el ejecutable
	 */
	std::string getBinPath(const std::string& name);
	std::string getBinPath(const char* name);

	/** Crea el path del fifo para un chico con pid <kid>
	 * @param kid pid del chico
	 * @return path del fifo para el chico de pid kid
	 */
	std::string buildKidFifoPath(pid_t kid);

}
#endif
