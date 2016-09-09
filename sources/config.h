#pragma once

#include <string>
#include <netinet/in.h>

struct Config {
	std::string hostName;
	in_port_t port;
	std::string name;
	std::string type;
	int imageId;
};

Config loadConfig();
void saveConfig(const Config &config);
