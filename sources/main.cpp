#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <boost/format.hpp>

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
		
		soc.sendData("abcde");
		soc.sendData(std::initializer_list<unsigned char>{9, 8, 7});
		
		unique_ptr<vector<unsigned char>> buf;
		while(buf = soc.getData()){
			cerr << boost::format("received %d bytes of data: \"%s\"") % buf->size() % string(begin(*buf), end(*buf)) << endl;
			for(auto c : *buf){
				cerr << boost::format("%02x ") % static_cast<int>(c);
			}
			cerr << endl;
		}
		
		saveConfig(config);
	} catch(SocketException &e) {
		cerr << boost::diagnostic_information(e) << endl;
	}
	
	return 0;
}
