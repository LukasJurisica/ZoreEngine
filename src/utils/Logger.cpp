#include "utils/Logger.hpp"

namespace Logger {
	void info(std::string s) {
		std::cout << s << std::endl;
	}

	void log(std::string s) {
		std::cout << "[Log] " << s << std::endl;
	}

	void error(std::string s) {
		std::cout << "[Error] " << s << std::endl;
		system("pause");
	}

	void crash(std::string s) {
		
	}
}