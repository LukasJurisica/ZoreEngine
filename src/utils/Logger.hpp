#pragma once

#include <iostream>

namespace Logger {
	void info(std::string s);
	void log(std::string s);
	void error(std::string s);
	void crash(std::string s);
}