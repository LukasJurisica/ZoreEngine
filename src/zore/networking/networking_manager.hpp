#pragma once

#include <string>

namespace zore::net {

	class Manager {
	public:
		static void Init();
		static void Free();
		static std::string GetHostName();

		static bool IsInitialized();
	};
}