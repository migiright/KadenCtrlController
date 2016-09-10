#pragma once

#include "Controller.h"

//AC100Vをスイッチするコントローラ
class Switcher : public Controller {
public:
	Switcher(std::weak_ptr<Socket> socket, std::weak_ptr<Config> config);
	virtual ~Switcher();
	
protected:
	virtual void processLocalData(BufferRange range);
};
