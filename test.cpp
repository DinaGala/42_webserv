#include "Response.hpp"
#include <iostream>

int main()
{
	Response resp;

	std::cout << resp.getResponse("404") << std::endl;
	resp.setBody("<html><h1>Hola</h1></html>");
	std::cout << resp.getResponse("404") << std::endl;
}
