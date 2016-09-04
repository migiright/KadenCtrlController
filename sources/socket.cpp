#include "socket.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cerrno>
#include <array>
#include <algorithm>
#include <assert.h>

using namespace std;

#define THROW BOOST_THROW_EXCEPTION

namespace {
	SocketException makeSocketException(string functionName){
		return SocketException()
			<< FunctionNameInfo(move(functionName))
			<< ErrnoInfo(errno)
			<< ErrorMessageInfo(strerror(errno));
	}
	
	template<class T>
	int bytesToInt(T iterator){
		return iterator[0] | iterator[1] << 8 | iterator[2] << 16 | iterator[3] << 24;
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
	, mask_()
	, maskWidth_(0)
{
	/* 変数の宣言 */
	hostent *server_ent; /* 接続先のホストの情報       */
	sockaddr_in server = {}; /* 接続先のソケットのアドレス */
	int soc; /* 通信に使うソケット         */
	fd_set mask;
	int width;

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
	
	//マスクの設定
	width = soc+1;
	FD_ZERO(&mask);
	FD_SET(soc, &mask);

	socket_ = soc;
	mask_ = mask;
	maskWidth_ = width;
}

Socket::~Socket(){
	if(socket_ >= 0) close(socket_);
	cerr << "closed." << endl;
}

unique_ptr<vector<unsigned char>> Socket::getData() const {

	unique_ptr<vector<unsigned char>> data;
	vector<unsigned char>::iterator itData;	
	array<unsigned char, 4> sizeBuf;
	auto itSizeBuf = begin(sizeBuf);
	do {
		fd_set readOk = mask_;
		if(select(maskWidth_, &readOk, nullptr, nullptr, nullptr) == -1){
			if(errno == EINTR){
				break;
			} else {
				THROW(makeSocketException("select"));
			}
			cerr << "received data piece" << endl;
		}
		if(FD_ISSET(socket_, &readOk)){ // ソケットにデータありか？
			//データをバッファに読み込み
			array<unsigned char, 0x100000> buf;
			ssize_t n = recv(socket_, &(buf[0]), buf.size(), MSG_PEEK);
			if(n < 0){
				THROW(makeSocketException("read"));
			} else if(n == 0){
				return nullptr;
			}
			auto endBuf = begin(buf) + n; //データが入っている場所の次をさす
			auto itBuf = begin(buf);
			//バッファから整形
			if(itSizeBuf != end(sizeBuf)){ //サイズがまだ読み終わってない
				auto toReadEnd = itBuf + min(endBuf-itBuf, end(sizeBuf)-itSizeBuf);
				itSizeBuf = copy(itBuf, toReadEnd, itSizeBuf);
				itBuf = toReadEnd;
				if(itSizeBuf == end(sizeBuf)){
					data = make_unique<vector<unsigned char>>(bytesToInt(sizeBuf.begin()));
					itData = begin(*data);
				}
			}
			if(itSizeBuf == end(sizeBuf)){ //サイズは読み終わっている
				auto toReadEnd = itBuf + min(endBuf-itBuf, end(*data)-itData);
				itData = copy(itBuf, toReadEnd, itData);
				itBuf = toReadEnd;
			}
			recv(socket_, &(buf[0]), itBuf - begin(buf), 0);
		}
		assert(FD_ISSET(socket_, &readOk) && "データがないのにselectを抜けた");
	} while(!data || itData != end(*data));
	return data;
}
