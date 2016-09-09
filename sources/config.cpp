#include "config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

using namespace std;
using namespace boost::property_tree;

namespace {
	constexpr const char *ConfigFileName = "config.ini";
}

Config loadConfig(){
    ptree pt;
	Config c = { //デフォルトの値を設定しておく
		"localhost"
		, 50000
		, "名無し"
		, "nofunc"
		, 0
	};
	
	try {
    	read_ini(ConfigFileName, pt);
	} catch(ini_parser_error &e){ //ファイルが読み込めなかった時はデフォルトで
		return c;
	}

	c.hostName = pt.get<string>("common.hostName", c.hostName);
	c.port = pt.get<in_port_t>("common.port", c.port);
	c.name = pt.get<string>("common.name", c.name);
	c.type = pt.get<string>("common.type", c.type);
	c.imageId = pt.get<int>("common.imageId", c.imageId);
	return c;
}

void saveConfig(const Config &config){
	ptree pt;
	
	pt.put("common.hostName", config.hostName);
	pt.put("common.port", config.port);
	pt.put("common.name", config.name);
	pt.put("common.type", config.type);
	pt.put("common.imageId", config.imageId);
	
	write_ini(ConfigFileName, pt);
}
