#include "NoFunc.h"

using namespace std;

NoFunc::NoFunc(std::weak_ptr<Socket> socket)
	: Controller(socket)
{
	cout << "Nofunc started." << endl;
}

void NoFunc::processLocalData(BufferRange range)
{
	cerr << "data received." << endl;
}
