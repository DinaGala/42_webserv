#include "Cluster.hpp"

int main(int argc, char *argv[]){

	try {
		Cluster clusterC;

		clusterC.setUpCluster();
		clusterC.runCluster();
	}
	catch (const std::exception e){
		std::cerr << e.what() << std::endl;
	}
	return (0);
}


/*	/*std::string buffer = "GET / HTTP/1.1\nHost: localhost:8080\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:127.0)\r\n";
    std::string buffer = "GET /hello.htm HTTP/1.1\n" \
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
		"Host: www.tutorialspoint.com\n" \
		"Accept-Language: en-us\n" \
		"Accept-Encoding: gzip, deflate\n" \
		"Connection: Keep-Alive\r\n";*/
		///Request request;
		//request.parseRequest(buffer);*/