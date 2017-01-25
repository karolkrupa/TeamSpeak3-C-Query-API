#define SUCCESS 0
#define ERROR 1

#include <iostream>
#include <Server.hpp>

using namespace std;

int main() {
	Ts3Api::Server server("TestTest", "7632");

	if(!server.connected())
		return 0;
	else
		return 1;
}