#pragma once

#include <string>

namespace zore {

	enum class HTTPMethod { GET, POST };

	//========================================================================
	//	An abstract generic socket class
	//========================================================================

	class Request {
	public:
		static std::string Make(const std::string& url, HTTPMethod method, int port = 80);
	};
}