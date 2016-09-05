#pragma once

#include <string>
#include <netinet/in.h>

struct Config {
	std::string hostName;
	in_port_t port;
	std::string type;
};

Config loadConfig();
