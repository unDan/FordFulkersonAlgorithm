#include <iostream>
#include "Network.h"

using std::cin;
using std::cout;
using std::endl;

int main()
{
	std::string filepath = "../TEST/test7.txt";
	Network network;
	network.readFrom(filepath);
	network.create();

	try
	{
		uint64_t maximal_flow = network.ford_fulkerson();
		cout << "Maximal flow for the current network is " << maximal_flow << endl << endl;
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl << endl;
	}

	return 0;
}