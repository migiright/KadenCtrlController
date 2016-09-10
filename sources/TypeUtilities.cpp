#include "TypeUtilities.h"

#include <stdexcept>
#include "config.h"
#include "NoFunc.h"
#include "Switcher.h"
#include "utilities.h"

using namespace std;

//configのtypeが存在するか調べる
bool isExistentType(const string &type){
	static unordered_set<string> table{
		"nofunc", "switcher"
	};
	return end(table) != table.find(type);
};

//configのtypeの文字列からインスタンスを作る
unique_ptr<Controller> createController(
	const string &type, weak_ptr<Socket> socket, weak_ptr<Config> config)
{
	static unordered_map<string, function<
		unique_ptr<Controller>(weak_ptr<Socket>, weak_ptr<Config>)
	>> table{
		{ "nofunc", [=](weak_ptr<Socket> soc, weak_ptr<Config> con){
			return make_unique<NoFunc>(soc, con);
		}}
		, {"switcher", [=](weak_ptr<Socket> soc, weak_ptr<Config> con){
			return make_unique<Switcher>(soc, con);
		}}
	};
	if(end(table) == table.find(type)){
		    BOOST_THROW_EXCEPTION(boost::enable_error_info(invalid_argument(
		    	(boost::format("type: %s is not found") % type).str()))
		    	<< ParameterNameInfo("type")
		    	<< ArgumentInfo(type));
	}

	return table[type](socket, config);
}

//デフォルトのコントローラーを作る
unique_ptr<Controller> createDefaultController(weak_ptr<Socket> socket, weak_ptr<Config> config)
{
	return make_unique<NoFunc>(socket, config);
}
