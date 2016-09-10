#include "config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "utilities.h"

using namespace std;
using namespace boost::property_tree;

LocalConfig::~LocalConfig()
{}

void LocalConfig::save(boost::property_tree::ptree &pt)
{}

namespace {
	constexpr const char *ConfigFileName = "config.ini";
	
	//タイプ固有のコンフィグをロード
	unique_ptr<LocalConfig> loadLocalConfig(const ptree &tree, string type){
		static unordered_map<string, function<unique_ptr<LocalConfig>(const ptree&)>> table{
			{"nofunc", [](const ptree &pt){ return make_unique<LocalConfig>(); }}
			, {"switcher", [](const ptree &pt){ return make_unique<LocalConfig>(); }}
		};
		if(end(table) == table.find(type)){
			return make_unique<LocalConfig>();
		}
		return table[type](tree);
	}
	
}

Config loadConfig(){
    ptree pt;
	Config c = { //デフォルトの値を設定しておく
		"localhost"
		, 50000
		, "名無し"
		, "nofunc"
		, 0
		, make_unique<LocalConfig>()
	};
	
	try {
    	read_ini(ConfigFileName, pt);
	} catch(ini_parser_error &e){ //ファイルが読み込めなかった時はデフォルトで
		c.local = loadLocalConfig(pt, c.type);
		return c;
	}

	c.hostName = pt.get<string>("common.hostName", c.hostName);
	c.port = pt.get<in_port_t>("common.port", c.port);
	c.name = pt.get<string>("common.name", c.name);
	c.type = pt.get<string>("common.type", c.type);
	c.imageId = pt.get<int>("common.imageId", c.imageId);
	
	c.local = loadLocalConfig(pt, c.type);
	
	return c;
}

void saveConfig(const Config &config){
	ptree pt;
	
	pt.put("common.hostName", config.hostName);
	pt.put("common.port", config.port);
	pt.put("common.name", config.name);
	pt.put("common.type", config.type);
	pt.put("common.imageId", config.imageId);
	
	config.local->save(pt);
	
	write_ini(ConfigFileName, pt);
}
