#include "Remocon.h"

#include <wiringPi.h>
#include "utilities.h"

using namespace std;

namespace {
	constexpr int GpioWritePin = 18;
	
	//ファイルをロードする
	//戻り値: 点灯時間と消灯時間(μs)のペア
	boost::optional<vector<tuple<int, int>>> loadFile(const string &file){
		vector<tuple<int, int>> r;
		ifstream is(file);
		for(int ln = 1; !(is.eof()); ++ln){
			string l;
			if(getline(is, l).bad()){ //1行読み込む
				cerr << "file read error. file: " << file << endl;
				return boost::none;
			}
			boost::algorithm::trim(l); //前後の空白を削除
			if(l.length() == 0) continue; //空白しかなかったら飛ばす
			istringstream lis(l); //読み込んだ1行をストリームにする
			int on, off;
			//onとoffの時間を読み取る
			//読み取れないか読み取った後に余計な文字があったらエラー
			if(!(lis >> on >> off && lis.eof())){
				cerr << "file syntax error. file: " << file << " line: " << ln << endl;
				cerr << l << endl;
				return boost::none;
			}
			r.push_back(make_tuple(on, off));
		}
		cerr << "file read: " << file << endl;
		return std::move(r);
	}
	
	//信号を送る
	void send(const vector<tuple<int, int>> &data, int repeat){
		for(int i = 0; i < repeat; ++i){
			for(auto r : data){
				pwmWrite(GpioWritePin, 84); //点灯
				delayMicroseconds(get<0>(r));
				pwmWrite(GpioWritePin, 0); //消灯
				delayMicroseconds(get<1>(r));
			}
		}
		cerr << "ir sent" << endl;
	}
}

Remocon::Remocon(std::weak_ptr<Socket> socket, std::weak_ptr<Config> config)
	: Controller(socket, config)
{
	pinMode(GpioWritePin, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(2); //19.2MHz / 2 / 253 = 37.94kHz ~ 38kHz
	pwmSetRange(253);
	cout << "Remocon started." << endl;
}

Remocon::~Remocon()
{
	digitalWrite(GpioWritePin, 0);
}

void Remocon::processLocalData(BufferRange range)
{
	if(begin(range) == end(range)){
		cerr << "Remocon: unknown message" << endl;
		return;
	}

	//受け取ったデータが0ならsend
	auto it = begin(range);
	switch(*it){
	case 0:
		{ //send
			++it;
			int repeat = *it;
			++it;
			auto f = readString(boost::make_iterator_range(it, end(range)));
			if(!f){
				cerr << "Remocon: unknown message" << endl;
				break;
			}
			auto d = loadFile(*f);
			if(!d) break;
			send(*d, repeat);
			cerr << "sent sygnal " << repeat << " times. file: " << f << endl;
			break;
		}
	case 1:
		cerr << "scan not inplemented" << endl;
		break;
	default:
		cerr << "Remocon: unknown message" << endl;
		break;
	}
}
