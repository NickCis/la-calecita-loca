#ifndef __SIGNAL_WAIT_H__
#define __SIGNAL_WAIT_H__

#include <signal.h>

class Signal {
	public:
		static int waitSignal(int signal);

	protected:
		Signal(){}
		~Signal(){}
};
#endif
