#include<iostream>
#include<cstdlib>

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "input format is wrong, example like this:\n"
			<< "./client <IPaddress>" << endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in cliaddr, servaddr
}
