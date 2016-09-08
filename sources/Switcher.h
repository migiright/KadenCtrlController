#pragma once

#include "Controller.h"

//AC100Vをスイッチするコントローラ
class Switcher : public Controller {
public:
	Switcher(std::weak_ptr<Socket> socket);
	virtual ~Switcher();
	
protected:
	virtual void processLocalData(BufferRange range);
};
