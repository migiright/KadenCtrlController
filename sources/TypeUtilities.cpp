#include "TypeUtilities.h"

#include <stdexcept>
#include "NoFunc.h"
#include "Switcher.h"

using namespace std;

//configのtypeが存在するか調べる
bool isExistentType(const string &type){
	static unordered_set<string> table{
		"nofunc", "switcher"
	};
	return end(table) != table.find(type);
};

//configのtypeの文字列からインスタンスを作る
unique_ptr<Controller> createController(const string &type, std::weak_ptr<Socket> socket)
{
	static unordered_map<string, function<unique_ptr<Controller>(std::weak_ptr<Socket>)>> table{
		{ "nofunc", [=](std::weak_ptr<Socket> soc){ return make_unique<NoFunc>(soc); } }
		, {"switcher", [=](std::weak_ptr<Socket> soc){ return make_unique<Switcher>(soc); } }
	};
	if(end(table) == table.find(type)){
		    BOOST_THROW_EXCEPTION(boost::enable_error_info(invalid_argument(
		    	(boost::format("type: %s is not found") % type).str()))
		    	<< ParameterNameInfo("type")
		    	<< ArgumentInfo(type));
	}

	return table[type](socket);
}

//デフォルトのコントローラーを作る
unique_ptr<Controller> createDefaultController(std::weak_ptr<Socket> socket)
{
	return make_unique<NoFunc>(socket);
}
