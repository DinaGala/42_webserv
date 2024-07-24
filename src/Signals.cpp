#include "Signals.hpp"

volatile bool signaled = true;

void signHandler(int sig) {
	if (sig == SIGQUIT || sig == SIGINT) {
		signaled = false;
	}
}

void initSignals() {
	signal(SIGINT, signHandler);
	signal(SIGQUIT, signHandler);
}
