#include "Switcher.h"

#include <wiringPi.h>
#include "utilities.h"

using namespace std;

namespace {
	constexpr int GpioPinNo = 14;
}

Switcher::Switcher(std::weak_ptr<Socket> socket)
	: Controller(socket)
{
	cout << "Switcher started." << endl;
	pinMode(GpioPinNo, OUTPUT);
}

Switcher::~Switcher()
{
	digitalWrite(GpioPinNo, 0);
}

void Switcher::processLocalData(BufferRange range)
{
	if(begin(range) == end(range)){
		cerr << "Switcher: unknown message" << endl;
		return;
	}
	
	//受け取ったデータが0ならOFF, 1ならON, それ以外は無視
	auto d = *begin(range);
	if(d == 0 || d == 1){
		int o = d == 0 ? 0 : 1;
		cerr << "received data: " << (o == 0 ? "OFF" : "ON") << endl;
		if(digitalRead(GpioPinNo) != o){
			digitalWrite(GpioPinNo, d == 0 ? 0 : 1);
			auto s = socket_.lock();
			array<unsigned char, 2> b{MessageTypeToUchar(MessageType::Local), d}; //
			s->sendData(b); //スイッチが切り替わったことを伝える
			cerr << "switched to " << (o == 0 ? "OFF." : "ON.") << endl;
		}
	}
}
