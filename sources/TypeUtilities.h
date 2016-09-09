#pragma once

#include "Controller.h"
#include <memory>

#include <boost/exception/all.hpp>

typedef boost::error_info<struct tag_ParameterName, std::string> ParameterNameInfo;
typedef boost::error_info<struct tag_Argument, std::string> ArgumentInfo;

//configのtypeが存在するか調べる
bool isExistentType(const std::string &type);

//configのtypeの文字列からインスタンスを作る
std::unique_ptr<Controller> createController(const std::string &type, std::weak_ptr<Socket> socket);

//デフォルトのコントローラーを作る
std::unique_ptr<Controller> createDefaultController(std::weak_ptr<Socket> socket);
