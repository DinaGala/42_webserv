#ifndef SIGNALS_HPP
# define SIGNALS_HPP

# include "Utils.hpp"

extern volatile bool signaled;

void	signHandler(int sig);
void	initSignals();

#endif
