#include "debug/Logger.hpp"
#include "debug/Debug.hpp"
#include "debug/Console.hpp"
#include <iostream>

namespace zore {

	void Logger::Log(const std::string& s) {
		DEBUG_ONLY(
			Console::SetTextColor(Console::Colour::Cyan);
			std::cout << s << std::endl;
		)
	}

	void Logger::Log(float v) {
		DEBUG_ONLY(
			Console::SetTextColor(Console::Colour::Cyan);
			std::cout << v << std::endl;
		)
	}

	void Logger::Info(const std::string& s) {
		DEBUG_ONLY(
			Console::SetTextColor(Console::Colour::Blue);
			std::cout << "[Info] " << s << std::endl;
		)
	}

	void Logger::Warn(const std::string& s) {
		DEBUG_ONLY(
			Console::SetTextColor(Console::Colour::Yellow);
			std::cout << "[Warn] " << s << std::endl;
		)
	}

	void Logger::Error(const std::string& s) {
		DEBUG_ONLY (
			Console::SetTextColor(Console::Colour::Red);
			std::cout << "[Error] " << s << std::endl;
		)
	}
}