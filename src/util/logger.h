#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

#ifndef GIT_REV
	#define GIT_REV "XXXX"
	#warning "No se seteo GIT_REV"
#endif

#ifndef TIME_COMPILED
	#define TIME_COMPILED "XXXX"
	#warning "No se seteo TIME_COMPILED"
#endif

class Logger {
	public:
		enum Mode {
			NORMAL,
			DEBUG
		};
		/** Metodo para imprimir salida de log.
		 * Se selecciona salida dependiendo del modo seteado, si es modo debug, imprime en el archivo, de ser modo normal imprime por salida standard
		 * @param fmt: formato de impresion (estilo printf)
		 * @param ... : resto de parametros (estilo printf)
		 */
		static void log(const std::string &fmt, ...);

		/** Setea el modo del logger
		 * @param mode: modo a asignar
		 */
		static void setMode(Mode mode);

		/** Setea el path del archivo de salida
		 * @param path: path a usar
		 */
		static void setPath(const std::string &path);

		/** Logea la informacion de compilacion.
		 * Es decir, se logea la revision del git y la hora en que se compilo.
		 */
		static void compileInfo();

		/** Setter para el nombre del proceso
		 */
		static void setName(const std::string &name);
		static void setName(const char* name_c);

		/** Logea la informacion de compilacion anteponiendole un texto.
		 * @param name: texto que se antepone al mensaje de la informacion de compilacion
		 * @see compileInfo
		 */
		static void compileInfo(const std::string &name);

	private:
		/** Variable que se utiliza para saber si ya se inicializo
		 */
		static bool isInit;

		/** Path del archivo de salida en modo debug
		 */
		static std::string path;

		/** Nombre del proceso
		 */
		static std::string name;

		/** Modo en que se encuentra configurado
		 */
		static Mode mode;
		Logger(){}
		~Logger(){}

		/** Inicializa a logger, por defecto levanta modo y path del archivo de salida desde las variables de ambiente, definidas con los defines ENVIROMENT_MODE (para el modo) y ENVIROMENT_PATH (para el archivo).
		 * Por defecto utiliza modo NORMAL y de archivo "./debug.log"
		 */
		static void init();
};

#endif
