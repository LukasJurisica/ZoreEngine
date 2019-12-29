#pragma once

#include <iostream>

namespace Logger {
	void log(const char* message);
	void error(const char* error);
	void crash(const char* error);
	void logString(std::string s);
}