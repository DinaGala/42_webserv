#include "Signals.hpp"

volatile bool signaled = true;

void signHandler(int sig) {
	std::cout << "IS INSIDE MANAGE SIGNALS" << std::endl;
	if (sig == SIGQUIT || sig == SIGINT) {
		signaled = false;
	}
	if (sig == SIGPIPE) {
		exit(1);
		std::cout << "IS INSIDE SIGPIPE" << std::endl;
	}
}

void initSignals() {
	signal(SIGINT, signHandler);
	signal(SIGQUIT, signHandler);
	signal(SIGPIPE, signHandler);
}
