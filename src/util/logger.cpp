#include "logger.h"

#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdarg.h>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;

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
				ofstream logfile;
				logfile.open(Logger::path.c_str(), ofstream::out | ofstream::app );
				logfile << text << endl;
				logfile.close();
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
	const char *mode_c = getenv(ENVIROMENT_MODE);
	if(mode_c){
		string mode = mode_c;
		if(mode == "DEBUG"){
			Logger::mode = DEBUG;
		}
	}

	Logger::path = "./debug.log";
	const char *path_c = getenv(ENVIROMENT_PATH);
	if(path_c){
		string path = path_c;
		if(!path_c || path == ""){
			Logger::path = path;
		}
	}

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
