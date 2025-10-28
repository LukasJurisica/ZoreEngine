#pragma once

#include <string>

namespace zore {

	//========================================================================
	//	Clipboard
	//========================================================================

	class Clipboard {
	public:
		static std::string Get();
		static void Set(const std::string& s);
	};
}