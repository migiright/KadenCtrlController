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
