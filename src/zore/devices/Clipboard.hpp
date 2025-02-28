#pragma once

#include <string>

namespace zore {

	class Clipboard {
	public:
		static std::string Get();
		static void Set(const std::string& s);
	};
}