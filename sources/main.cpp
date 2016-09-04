#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "config.h"
#include "socket.h"

using namespace std;

namespace {
	volatile sig_atomic_t receivedSignal = 0;;
}

//シグナルハンドラ ctrl+cが押された時に呼ばれる
void signalHandler(int signum){
	receivedSignal = 1;
}

int main(int argc, char **argv){
	
	{ //シグナルハンドラの登録
		struct sigaction sa;
		sa.sa_handler = signalHandler;
		sa.sa_flags = SA_RESTART;
		sigaction(SIGINT, &sa, nullptr);
	}
	
	Config config = loadConfig();
	
	try {
		Socket soc(config.hostName, config.port);
	} catch(SocketException &e) {
		cerr << boost::diagnostic_information(e) << endl;
	}
	
	return 0;
}
