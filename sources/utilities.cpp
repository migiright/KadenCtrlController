#include "utilities.h"

using namespace std;;

boost::optional<std::string> readString(boost::any_range<
	unsigned char, boost::forward_traversal_tag, const unsigned char&, std::ptrdiff_t
> range){
	if(distance(range) <= 4) return boost::none;
	auto it = begin(range);
	int len = bytesToInt(it); //ファイル名の文字数
	std::advance(it, 4);
	if(distance(it, end(range)) < len) return boost::none;
	std::string r;
	insert(r, r.begin(), boost::make_iterator_range(it, std::next(it, len)));
	return r;
}
