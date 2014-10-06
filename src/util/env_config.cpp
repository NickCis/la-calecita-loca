#include "env_config.h"

#include <stdlib.h>


int Config::getInt(const std::string& name, const int& value){
	int var = value;
	const char *var_c = getenv(name.c_str());
	if(var_c)
		var = atoi(var_c);

	return var;
}

std::string Config::getStr(const std::string& name, const std::string& value){
	std::string var = value;
	const char *var_c = getenv(name.c_str());
	if(var_c)
		var = var_c;

	return var;
}
