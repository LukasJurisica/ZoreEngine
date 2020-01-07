#pragma once

#include <String>
#include <vector>

class StringUtils {
public:
	static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
	static std::string strip(const std::string& str, const std::string& delimiter);
};