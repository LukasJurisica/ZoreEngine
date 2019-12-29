#include "utils/Logger.hpp"

namespace Logger {
	void log(const char* message) {
		std::cout << "[Log] " << message << std::endl;
	}

	void error(const char* error) {
		std::cout << "[Error] " << error << std::endl;
		system("pause");
	}

	void crash(const char* error) {
		
	}

	void logString(std::string s) {
		std::cout << s << std::endl;
	}
}