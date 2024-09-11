#include "zore/utils/StringUtils.hpp"

namespace zore {

	void StringUtils::Split(std::vector<std::string>& result, const std::string& str, const std::string& delimiter) {
		size_t start = 0, end;
		do {
			end = str.find_first_of(delimiter, start);
			result.emplace_back(str.substr(start, end - start));
			start = str.find_first_not_of(delimiter, end);
		} while (start != std::string::npos);
	}

	void StringUtils::SplitView(std::vector<std::string_view>& result, const std::string& str, const std::string& delimiter) {
		std::string_view str_view(str);
		size_t start = 0, end;
		do {
			end = str.find_first_of(delimiter, start);
			result.emplace_back(str_view.substr(start, end - start));
			start = str.find_first_not_of(delimiter, end);
		} while (start != std::string::npos);
	}

	std::wstring StringUtils::to_wstring(const std::string& string) {
		return std::wstring(string.begin(), string.end());
	}

	std::string StringUtils::to_string(const std::wstring& string) {
		return std::string(string.begin(), string.end());
	}
}