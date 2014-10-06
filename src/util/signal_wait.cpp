#include "signal_wait.h"

#include <stdlib.h>

int Signal::waitSignal(int signal){
	sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, signal);
	sigprocmask(SIG_BLOCK, &mask, NULL);

	int sig;
	if(sigwait(&mask, &sig) == 0){
		return sig;
	}

	return 0;
}
