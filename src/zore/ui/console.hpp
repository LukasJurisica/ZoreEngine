#pragma once

#include <string>
#include <vector>

namespace zore {

	class Console {
	public:
		enum class LogLevel { LOG, INFO, WARN, ERR, CMD };

	public:
		static void Draw();
		static void Print(const std::string& message, LogLevel level = LogLevel::LOG);
		static void Clear();
		static void Dump();
	};
}