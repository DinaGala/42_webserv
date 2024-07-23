#ifndef SIGNALS_HPP
# define SIGNALS_HPP

# include "Utils.hpp"

class Signals {
	
	private:
		Signals();
	public:
		~Signals();
		static void	signHandler(int sig);
		static void	initSignals();
		static bool signaled;
};

#endif
