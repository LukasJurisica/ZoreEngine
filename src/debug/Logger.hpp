#pragma once
#include <string>

namespace zore {

	class Logger {
	public:
		static void Log(const std::string& s);
		static void Log(float f);
		static void Info(const std::string& s);
		static void Warn(const std::string& s);
		static void Error(const std::string& s);
	};
}