//ソケット関係の関数群
#pragma once

#include <netinet/in.h>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/exception/all.hpp>

//サーバーに繋ぐ
//hostname 接続先のホスト名, port: 接続先のポート番号
int setupClient(const std::string &hostname, in_port_t port);

class SocketException : public boost::exception, public std::exception {};
typedef boost::error_info<struct tag_FunctionName, std::string> FunctionNameInfo;
typedef boost::error_info<struct tag_Errno, int> ErrnoInfo;
typedef boost::error_info<struct tag_Herrno, int> HerrnoInfo;
typedef boost::error_info<struct tag_ErrorMessage, std::string> ErrorMessageInfo;

class Socket : boost::noncopyable {
public:
	//サーバーに繋ぐ
	//hostname 接続先のホスト名, port: 接続先のポート番号
	Socket(const std::string &hostname, in_port_t port);
	//closeする
	virtual ~Socket();
	
	int socket() const { return socket_; }
private:
	int socket_;
};

