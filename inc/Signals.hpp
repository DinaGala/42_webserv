#pragma once

#ifndef SIGNALS_HPP
# define SIGNALS_HPP

# include <signal.h>

extern volatile bool signaled;

void	signHandler(int sig);
void	initSignals();

#endif
