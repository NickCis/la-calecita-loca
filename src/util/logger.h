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

		/** Logea la informacion de compilacion anteponiendole un texto.
		 * @param name: texto que se antepone al mensaje de la informacion de compilacion
		 * @see compileInfo
		 */
		static void compileInfo(const std::string &name);

	private:
		static bool isInit;
		static std::string path;
		static Mode mode;
		Logger(){}
		~Logger(){}

		/** Inicializa a logger, por defecto levanta modo y path del archivo de salida desde las variables de ambiente, definidas con los defines ENVIROMENT_MODE (para el modo) y ENVIROMENT_PATH (para el archivo).
		 * Por defecto utiliza modo NORMAL y de archivo "./debug.log"
		 */
		static void init();
};

#endif
