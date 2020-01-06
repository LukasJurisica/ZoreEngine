#include "utils/StringUtils.hpp"

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) const {
	std::vector<std::string> result = { };
	size_t prev = 0;
	for (int i = 0; i < str.length(); i++) {
		if (delimiter.find(str.at(i)) != -1)
			result.push_back(str.substr(prev, i));
	}
	result.push_back(str.substr(prev, str.length()));
	return result;
}