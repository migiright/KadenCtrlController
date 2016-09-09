#pragma once

#include <array>

template<class T>
int bytesToInt(T iterator){
	return iterator[0] | iterator[1] << 8 | iterator[2] << 16 | iterator[3] << 24;
}

inline std::array<unsigned char, 4> intToBytes(unsigned x){
	return { static_cast<unsigned char>(x & 0xff)
		, static_cast<unsigned char>((x>>8) & 0xff)
		, static_cast<unsigned char>((x>>16) & 0xff)
		, static_cast<unsigned char>((x>>24) & 0xff)
	};
}

//サーバーに送るメッセージのタイプ
enum class MessageType : unsigned char {
	Info //info(name, type, imageId)
	, Local //typeごとに違うデータを送る
};

constexpr unsigned char MessageTypeToUchar(MessageType t) {
	return static_cast<unsigned char>(t);
}

//サーバーから受け取るメッセージのタイプ
enum class ReceivingMessageType : unsigned char {
	Local //typeごとに違うデータを受け取る
};

constexpr ReceivingMessageType ucharToReceivingMessageType(unsigned char t){
	return static_cast<ReceivingMessageType>(t);
}
