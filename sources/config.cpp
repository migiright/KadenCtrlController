#include "config.h"
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

using namespace std;
using namespace boost::property_tree;

Config loadConfig(){
    ptree pt;
	Config c = { //デフォルトの値を設定しておく
		"localhost"
		, 50000
		, "nofunc"
	};
	
	try {
    	read_ini("config.ini", pt);
	} catch(ini_parser_error &e){ //ファイルが読み込めなかった時はデフォルトで
		return c;
	}

	c.hostName = pt.get<string>("common.hostName", c.hostName);
	c.port = pt.get<in_port_t>("common.port", c.port);
	c.type = pt.get<string>("common.type", c.type);
	return c;
}
