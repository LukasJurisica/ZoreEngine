#pragma once

#include <String>
#include <vector>

class StringUtils {
public:
	std::vector<std::string> split(const std::string& str, const std::string& delimiter) const;

};