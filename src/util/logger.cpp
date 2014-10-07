#include "logger.h"
#include "defines.h"
#include "env_config.h"
#include "lock_file.h"

#include <iostream>
#include <sstream>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

using std::string;
using std::cout;
using std::endl;
using std::stringstream;

bool Logger::isInit = false;
Logger::Mode Logger::mode = Logger::NORMAL;
string Logger::path = "";

void Logger::log(const string &fmt, ...){
	Logger::init();
	char text_c[512];

	va_list args;
	va_start(args, fmt);
	vsnprintf(text_c, 512, fmt.c_str(), args);
	va_end(args);
	text_c[511] = 0;

	string text = text_c;

	switch(Logger::mode){
		case DEBUG:
			{
				stringstream ss;
				ss << "[" << getpid() << "] " << text << endl;
				const char *txt = ss.str().c_str();
				LockFile file(Logger::path.c_str());
				file.tomarLock();
				file.escribir(txt, strlen(txt));
				file.liberarLock();
			}
			break;
		case NORMAL:
		default:
			cout << text << endl;
			break;
	}
}

void Logger::setMode(Mode mode){
	Logger::init();
	Logger::mode = mode;
}

void Logger::setPath(const string &path){
	Logger::init();
	Logger::path = path;
}

void Logger::init(){
	if(Logger::isInit)
		return;

	Logger::mode = NORMAL;
	string mode = Config::getStr(ENVIROMENT_MODE, "NORMAL");
	if(mode == "DEBUG"){
		Logger::mode = DEBUG;
	}

	Logger::path = Config::getStr(ENVIROMENT_PATH, DEFAULT_LOG_FILE);
	Logger::isInit = true;
}

void Logger::compileInfo(){
	Logger::init();
	Logger::log("git: '" GIT_REV "' date: '" TIME_COMPILED "'");
}

void Logger::compileInfo(const string &name){
	Logger::init();
	Logger::log("%s: git: '" GIT_REV "' date: '" TIME_COMPILED "'", name.c_str());
}
