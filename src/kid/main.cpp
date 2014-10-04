#include "../util/logger.h"
#include <unistd.h>

int main( int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused))){
	Logger::compileInfo("KID");
	sleep(2);
	Logger::log("KID", "End");
	return 0;
}
