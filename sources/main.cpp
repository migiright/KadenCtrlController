#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <boost/format.hpp>

#include "config.h"
#include "socket.h"
#include "utilities.h"

using namespace std;

namespace {
	
	volatile sig_atomic_t receivedSignal = 0;;

	//シグナルハンドラ ctrl+cが押された時に呼ばれる
	void signalHandler(int signum){
		receivedSignal = 1;
	}
	
	//vectorに文字数と文字列を追加
	void addString(vector<unsigned char> &v, const string& s){
		boost::insert(v, end(v), intToBytes(s.size()));
		boost::insert(v, end(v), s);
	}
	
	//info(name, type, imageId)を送るデータを作る
	vector<unsigned char> makeInfo(string name, string type, int imageId) {
		vector<unsigned char> b;
		
		b.push_back(MessageTypeToUchar(MessageType::Info));
		addString(b, name);
		addString(b, type);
		boost::insert(b, end(b), intToBytes(imageId));
		
		return move(b);
	}
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
		
		//infoを送る
		soc.sendData(makeInfo(config.name, config.type, config.imageId));
		
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
