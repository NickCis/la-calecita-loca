#include "env_config.h"

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

#include <sstream>

using std::string;
using std::stringstream;

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

string Config::getBinPath(){
	char buf[512];
	char linkname[64]; /* /proc/<pid>/exe */
	pid_t pid;
	int ret;
	string path = "";

	pid = getpid();

	if (snprintf(linkname, sizeof(linkname), "/proc/%i/exe", pid) < 0){
		return path;
	}

	/* Now read the symbolic link */
	ret = readlink(linkname, buf, 512);

	/* In case of an error, leave the handling up to the caller */
	if (ret == -1)
		return NULL;

	/* Report insufficient buffer size */
	if (ret >= 512) {
		errno = ERANGE;
		return path;
	}

	/* Ensure proper NUL termination */
	buf[ret] = 0;

	path = dirname(buf);

	return path;
}

string Config::getBinPath(const char* name){
	string n = name;
	return Config::getBinPath(n);
}

string Config::getBinPath(const string& name){
	stringstream ss;
	ss << Config::getBinPath() << "/" << name;
	return ss.str();
}

string Config::buildKidFifoPath(pid_t kid){
	stringstream ss;
	ss << KID_FIFO << myPid;
	return ss.str();
}
