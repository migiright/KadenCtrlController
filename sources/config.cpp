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
		, "nofunc"
	};
	
	try {
    	read_ini(ConfigFileName, pt);
	} catch(ini_parser_error &e){ //ファイルが読み込めなかった時はデフォルトで
		return c;
	}

	c.hostName = pt.get<string>("common.hostName", c.hostName);
	c.port = pt.get<in_port_t>("common.port", c.port);
	c.type = pt.get<string>("common.type", c.type);
	return c;
}

void saveConfig(const Config &config){
	ptree pt;
	
	pt.put("common.hostName", config.hostName);
	pt.put("common.port", config.port);
	pt.put("common.type", config.type);
	
	write_ini(ConfigFileName, pt);
}
