#pragma once

#include <string>
#include <netinet/in.h>
#include <memory>
#include <boost/property_tree/ptree_fwd.hpp>

class LocalConfig {
public:
	virtual ~LocalConfig();
	virtual void save(boost::property_tree::ptree &pt);
};

struct Config {
	std::string hostName;
	in_port_t port;
	std::string name;
	std::string type;
	int imageId;
	std::unique_ptr<LocalConfig> local;
};

Config loadConfig();
void saveConfig(const Config &config);
