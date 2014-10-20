#include "logger.h"
#include "defines.h"
#include "env_config.h"
#include "lock_file.h"

#include <iostream>
#include <sstream>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

using std::string;
using std::cout;
using std::endl;
using std::stringstream;

bool Logger::isInit = false;
Logger::Mode Logger::mode = Logger::NORMAL;
string Logger::path = "";
string Logger::name = "";

void Logger::log(const string &fmt, ...){
	Logger::init();
	char text_c[512];
	char time_text[512];

	va_list args;
	va_start(args, fmt);
	vsnprintf(text_c, 512, fmt.c_str(), args);
	va_end(args);
	text_c[511] = 0;

	string text = text_c;

	switch(Logger::mode){
		case DEBUG:
			{
				LockFile file(Logger::path.c_str());
				file.tomarLock();
				time_t t = time(NULL);
				strftime(time_text, 512, "%T", localtime(&t));
				stringstream ss;
				ss << time_text << " [" << getpid() << "] ";
				if(Logger::name != "")
					ss << name << ": ";
				ss << text << endl;
				const char *txt = ss.str().c_str();
				file.write(txt, strlen(txt));
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
	Logger::log("git: '" GIT_REV "' compile date: '" TIME_COMPILED "'");
}

void Logger::setName(const char* name_c){
	string name = name_c;
	Logger::setName(name);
}

void Logger::setName(const string& name){
	Logger::name = name;
}
