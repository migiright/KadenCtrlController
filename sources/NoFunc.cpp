#include "NoFunc.h"

using namespace std;

NoFunc::NoFunc(std::weak_ptr<Socket> socket, weak_ptr<Config> config)
	: Controller(socket, config)
{
	cout << "Nofunc started." << endl;
}

void NoFunc::processLocalData(BufferRange range)
{
	cerr << "data received." << endl;
}
