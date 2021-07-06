#pragma once
#include <string>
#include <vector>

namespace zore {

	class StringUtils {
	public:
		static void SplitOnChr(std::vector<std::string>& result, const std::string& str, const std::string& delimiter);
		static void SplitOnStr(std::vector<std::string>& result, const std::string& str, const std::string& delimiter, bool inclusive = false);
		static std::string Strip(const std::string& str, const std::string& delimiter);
		static std::wstring to_wstring(const std::string& string);
		static std::string to_string(const std::wstring& string);
	};
}