#include <iostream>
#include <unistd.h>

#include "config.h"
#include "socket.h"

using namespace std;

int main(int argc, char **argv){
	Config config = loadConfig();
	
	int soc = setupClient(config.hostName, config.port);
	close(soc);
	
	return 0;
}
