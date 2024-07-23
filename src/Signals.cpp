#include "Signals.hpp"

bool Signals::signaled = true;

Signals::Signals(){}

Signals::~Signals(){
}

void Signals::signHandler(int sig) {
    if (sig == SIGQUIT || sig == SIGINT) {
        std::cout << "Signal Handler"<< std::endl;
        signaled = true;
    }
}

void Signals::initSignals() {
	signal(SIGINT, signHandler);
	signal(SIGQUIT, signHandler);
}