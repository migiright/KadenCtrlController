#include "Controller.h"

#include <wiringPi.h>
#include "utilities.h"

using namespace std;

Controller::Controller(std::weak_ptr<Socket> socket, std::weak_ptr<Config> config)
	: socket_(socket)
	, config_(config)
{
	wiringPiSetupGpio();
}

Controller::~Controller()
{
}

void Controller::processData(BufferRange range)
{
	switch(ucharToReceivingMessageType(*begin(range))){
	case ReceivingMessageType::Local:
		processLocalData(boost::make_iterator_range(make_pair(std::next(begin(range)), end(range))));
		break;
	default:
		cerr << "controller: unknown message: " << *begin(range) << endl;
	}
}
