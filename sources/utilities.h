#pragma once

#include <array>
#include <boost/exception/all.hpp>
#include <boost/optional.hpp>
#include <boost/range/any_range.hpp>

typedef boost::error_info<struct tag_ParameterName, std::string> ParameterNameInfo;
typedef boost::error_info<struct tag_Argument, std::string> ArgumentInfo;

template<class T>
int bytesToInt(T iterator){
	int x = 0;
	x += *iterator++;
	x += *iterator++ << 8;
	x += *iterator++ << 16;
	x += *iterator++ << 24;
	return x;
}

inline std::array<unsigned char, 4> intToBytes(unsigned x){
	return { static_cast<unsigned char>(x & 0xff)
		, static_cast<unsigned char>((x>>8) & 0xff)
		, static_cast<unsigned char>((x>>16) & 0xff)
		, static_cast<unsigned char>((x>>24) & 0xff)
	};
}

//unsigned charのrangeから文字数を読み取ってstringを読み取る
boost::optional<std::string> readString(boost::any_range<
	unsigned char, boost::forward_traversal_tag, const unsigned char&, std::ptrdiff_t
> range);

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
