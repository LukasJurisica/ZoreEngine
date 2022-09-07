#include "debug/Logger.hpp"
#include "debug/Debug.hpp"
#include "debug/Console.hpp"
#include <iostream>

namespace zore {

	template<typename T>
	void Logger::Log(T value) {
		DEBUG_ONLY(
			Console::SetTextColor(Console::Colour::Cyan);
			std::cout << value << std::endl;
		)
	}

	template void Logger::Log(std::string);
	template void Logger::Log(char const*);
	template void Logger::Log(bool);
	template void Logger::Log(char);
	template void Logger::Log(unsigned char);
	template void Logger::Log(short);
	template void Logger::Log(unsigned short);
	template void Logger::Log(int);
	template void Logger::Log(unsigned int);
	template void Logger::Log(long long);
	template void Logger::Log(unsigned long long);
	template void Logger::Log(float);
	template void Logger::Log(double);

	void Logger::Info(const std::string& s) {
		DEBUG_ONLY(
			Console::SetTextColor(Console::Colour::Green);
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