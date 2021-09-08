#pragma once
#include <string>
#include <vector>

namespace zore {

	class StringUtils {
	public:
		static void SplitOnChr(std::vector<std::string>& result, const std::string& str, const std::string& delimiter);
		static void SplitOnStr(std::vector<std::string>& result, const std::string& str, const std::string& delimiter, bool inclusive = false);

		static inline std::string LTrim(const std::string& s, const std::string& delimiter = " \n\r\t") {
			return s.substr(s.find_first_not_of(delimiter), std::string::npos);
		}

		static inline void LTrimInPlace(std::string& s, const std::string& delimiter = " \n\r\t") {
			s.erase(0, s.find_first_not_of(delimiter));
		}

		static inline std::string RTrim(const std::string& s, const std::string& delimiter = " \n\r\t") {
			return s.substr(0, s.find_last_not_of(delimiter) + 1);
		}

		static inline void RTrimInPlace(std::string& s, const std::string& delimiter = " \n\r\t") {
			s.erase(s.find_last_not_of(delimiter) + 1, std::string::npos);
		}

		static inline std::string Trim(const std::string& s, const std::string& delimiter = " \n\r\t") {
			return s.substr(s.find_first_not_of(delimiter), s.find_last_not_of(delimiter) + 1);
		}

		static inline void TrimInPlace(std::string& s, const std::string& delimiter = " \n\r\t") {
			LTrimInPlace(s, delimiter);
			RTrimInPlace(s, delimiter);
		}

		

		static std::wstring to_wstring(const std::string& string);
		static std::string to_string(const std::wstring& string);
	};
}