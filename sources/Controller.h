#pragma once

#include <boost/noncopyable.hpp>
#include <boost/range/any_range.hpp>

#include "socket.h"
#include "config.h"

//各コントローラーのタイプに特有の処理をするために継承されるクラス
class Controller : boost::noncopyable {
public:
	Controller(std::weak_ptr<Socket> socket, std::weak_ptr<Config> config);
	virtual ~Controller();
	
	//サーバーから送られてきたデータを処理する
	using BufferRange = boost::any_range<
		const unsigned char, boost::forward_traversal_tag, const unsigned char&, std::ptrdiff_t
	>;
	void processData(BufferRange range);
	
protected:
	
	virtual void processLocalData(BufferRange range) = 0;
	const std::weak_ptr<Socket> socket_;
	const std::weak_ptr<Config> config_;
};
