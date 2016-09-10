#pragma once

#include "Controller.h"
#include <memory>

//configのtypeが存在するか調べる
bool isExistentType(const std::string &type);

//configのtypeの文字列からインスタンスを作る
std::unique_ptr<Controller> createController(const std::string &type, std::weak_ptr<Socket> socket, std::weak_ptr<Config> config);

//デフォルトのコントローラーを作る
std::unique_ptr<Controller> createDefaultController(std::weak_ptr<Socket> socket, std::weak_ptr<Config> config);
