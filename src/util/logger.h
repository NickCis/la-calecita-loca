#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

#define ENVIROMENT_MODE "CALECITA_MODE"
#define ENVIROMENT_PATH "CALECITA_PATH"

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
		static void log(std::string text);
		static void log(std::string name, std::string text);
		static void setMode(Mode mode);
		static void setPath(std::string path);
		static void compileInfo();
		static void compileInfo(std::string name);

	private:
		static bool isInit;
		static std::string path;
		static Mode mode;
		Logger(){}
		~Logger(){}
		static void init();
};

#endif
