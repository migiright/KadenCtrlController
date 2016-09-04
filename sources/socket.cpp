#include "socket.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cerrno>

using namespace std;

#define THROW BOOST_THROW_EXCEPTION

namespace {
	SocketException makeSocketException(string functionName){
		return SocketException()
			<< FunctionNameInfo(move(functionName))
			<< ErrnoInfo(errno)
			<< ErrorMessageInfo(strerror(errno));
	}
}

int setupClient(const string &hostname, in_port_t port)
	/* 戻り値   ソケットのディスクリプタ */
	/* hostname 接続先のホスト名         */
	/* port     接続先のポート番号       */
{
	/* 変数の宣言 */
	hostent *server_ent; /* 接続先のホストの情報       */
	sockaddr_in server = {}; /* 接続先のソケットのアドレス */
	int soc; /* 通信に使うソケット         */

	/* 接続先のホスト名からホストの情報を得る */
	if((server_ent = gethostbyname(hostname.c_str())) == NULL){
		perror("gethostbyname");
		return -1;
	}

	/* 接続先のアドレスを sockaddr_in 構造体に設定 */
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	memcpy(&server.sin_addr, server_ent->h_addr, server_ent->h_length);

	/* IPv4 でストリーム型のソケットを作成 */
	if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		return -1;
	}

	/* 接続 */
	if (connect(soc, reinterpret_cast<sockaddr*>(&server), sizeof(server)) == -1){
		perror("connect");
		return -1;
	}
	cerr << "connected." << endl;

	/* ソケットを返す */
	return soc;
}

Socket::Socket(const std::string &hostname, in_port_t port)
	: socket_(-1)
{
	/* 変数の宣言 */
	hostent *server_ent; /* 接続先のホストの情報       */
	sockaddr_in server = {}; /* 接続先のソケットのアドレス */
	int soc; /* 通信に使うソケット         */

	/* 接続先のホスト名からホストの情報を得る */
	if((server_ent = gethostbyname(hostname.c_str())) == NULL){
		THROW(SocketException()
			<< FunctionNameInfo("gethostbyname")
			<< HerrnoInfo(h_errno)
			<< ErrorMessageInfo(hstrerror(h_errno)));
	}

	/* 接続先のアドレスを sockaddr_in 構造体に設定 */
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	memcpy(&server.sin_addr, server_ent->h_addr, server_ent->h_length);

	/* IPv4 でストリーム型のソケットを作成 */
	if ((soc = ::socket(AF_INET, SOCK_STREAM, 0)) < 0){
		THROW(makeSocketException("socket"));
	}

	/* 接続 */
	if (connect(soc, reinterpret_cast<sockaddr*>(&server), sizeof(server)) == -1){
		THROW(makeSocketException("connect"));
	}
	cerr << "connected." << endl;

	socket_ = soc;
}

Socket::~Socket(){
	if(socket_ >= 0) close(socket_);
	cerr << "closed." << endl;
}
