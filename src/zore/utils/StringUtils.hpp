#pragma once
#include <string>
#include <vector>

namespace zore {

	class StringUtils {
	public:
		static void Split(std::vector<std::string>& result, const std::string& str, const std::string& delimiter);
		static void SplitView(std::vector<std::string_view>& result, const std::string& str, const std::string& delimiter);

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

		static std::string ToLower(const std::string& s) {
			std::string result = s;
			for (char& c : result)
				c = std::tolower(c);
			return result;
		}

		static void ToLowerInPlace(std::string& s) {
			for (char& c : s)
				c = std::tolower(c);
		}

		static std::string ToUpper(const std::string& s) {
			std::string result = s;
			for (char& c : result)
				c = std::toupper(c);
			return result;
		}

		static void ToUpperInPlace(std::string& s) {
			for (char& c : s)
				c = std::toupper(c);
		}

		static inline std::string SubStrRange(const std::string& s, size_t left, size_t right = std::string::npos) {
			return s.substr(left, right - left);
		}

		static inline std::string ToStrMinDigits(int value, int count) {
			std::string num = std::to_string(value);
			count -= static_cast<int>(num.length());
			return std::string(count, '0') + num;
		}

		static std::wstring to_wstring(const std::string& string);
		static std::string to_string(const std::wstring& string);

	};
}