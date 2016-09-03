//ソケット関係の関数群
#pragma once

#include <netinet/in.h>
#include <string>

//サーバーに繋ぐ
//hostname 接続先のホスト名, port: 接続先のポート番号
int setupClient(const std::string &hostname, in_port_t port);
