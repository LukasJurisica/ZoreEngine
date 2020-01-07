#include "utils/StringUtils.hpp"
#include "utils/Logger.hpp"

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> result;
	size_t start, end = 0;

	while (end != std::string::npos) {
		start = str.find_first_not_of(delimiter, end);
		if (start == std::string::npos)
			return result;
		end = str.find_first_of(delimiter, start);
		std::string res = str.substr(start, end - start);
		result.push_back(res);
	}

	return result;
}