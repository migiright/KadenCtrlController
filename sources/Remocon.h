#pragma once

#include "Controller.h"
#include <memory>

//赤外線リモコン
class Remocon : public Controller {
public:
	Remocon(std::weak_ptr<Socket> socket, std::weak_ptr<Config> config);
	virtual ~Remocon();
	
protected:
	virtual void processLocalData(BufferRange range);
};
