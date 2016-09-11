#pragma once

#include "Controller.h"

//何もしないコントローラ
//デフォルト
class NoFunc : public Controller {
public:
	NoFunc(std::weak_ptr<Socket> socket, std::weak_ptr<Config> config);
	
protected:
	virtual void processLocalData(BufferRange range);
};
