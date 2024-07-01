#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
/*
int main(void)
{
	std::map<int, std::string> mp;
	std::map<int, std::string> mp2;

	mp[1] = "one";
	mp[2] = "two";
	mp[3] = "three";

	mp2 = mp;

	std::cout << "wth" << std::endl;
	for(std::map<int, std::string>::iterator it = mp.begin(); it != mp.end(); it++)
		std::cout << "mp key " << it->first << " pair " << it->second << std::endl;
	for(std::map<int, std::string>::iterator it = mp2.begin(); it != mp2.end(); it++)
		std::cout << "mp2 key " << it->first << " pair " << it->second << std::endl;
	std::cout << "wtf" << std::endl;

	std::string	str = "";

	if (str.empty() == true)
		std::cout << "empty" << std::endl;
	else
		std::cout << "filled" << std::endl;

    return 0;
}
*/
#include <ctime>
#include <iostream>

int main()
{
    std::time_t result = std::time(NULL);
	std::cout << std::asctime(std::localtime(std::time(NULL)));
    std::cout << std::asctime(std::localtime(&result));
}
