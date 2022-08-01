#include "utils/StringUtils.hpp"
#include "debug/Logger.hpp"

namespace zore {

	void StringUtils::SplitOnChr(std::vector<std::string>& result, const std::string& str, const std::string& delimiter) {
		size_t start = 0, end;
		do {
			end = str.find_first_of(delimiter, start);
			result.emplace_back(str.substr(start, end - start));
			start = str.find_first_not_of(delimiter, end);
		} while (end != std::string::npos);
	}

	void StringUtils::SplitOnStr(std::vector<std::string>& result, const std::string& str, const std::string& delimiter, bool inclusive) {
		size_t searchStart = 0, stringStart = 0, end;
		do {
			end = str.find(delimiter, searchStart);
			result.emplace_back(str.substr(stringStart, end - stringStart));
			searchStart = end + delimiter.length();
			stringStart = inclusive ? end : searchStart;
		} while (end != std::string::npos); 
	}

	std::wstring StringUtils::to_wstring(const std::string& string) {
		//std::wstring result(string.length(), L' ');
		//std::copy(string.begin(), string.end(), result.begin());
		return std::wstring(string.begin(), string.end());
	}

	std::string StringUtils::to_string(const std::wstring& string) {
		return std::string(string.begin(), string.end());
	}
}