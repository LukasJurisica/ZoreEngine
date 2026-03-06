#pragma once
#include <string>
#include <vector>
#include <cctype>

namespace zore {

	class String {
	public:
		static inline size_t Count(std::string_view s, char delimiter) {
			size_t count = 0;
			const char* ptr = s.data();
			const char* end = ptr + s.size();

			while (ptr < end) {
				const char* pos = (const char*)memchr(ptr, delimiter, end - ptr);
				if (!pos)
					break;
				count++;
				ptr = pos + 1;
			}
			return count;
		}

		static inline void Split(std::vector<std::string>& result, std::string_view str, char delimiter) {
			result.reserve(result.size() + Count(str, delimiter));
			size_t start = 0, end;
			while ((end = str.find(delimiter, start)) != std::string::npos) {
				result.emplace_back(str.substr(start, end - start));
				start = end + 1;
			}
			result.emplace_back(str.substr(start));
		}

		static inline void Split(std::vector<std::string>& result, std::string_view str, std::string_view delimiter = " \n\r\t") {
			size_t start = 0, end;
			do {
				end = str.find_first_of(delimiter, start);
				result.emplace_back(str.substr(start, end - start));
				start = str.find_first_not_of(delimiter, end);
			} while (start != std::string::npos);
		}

		static inline std::vector<std::string> Split(std::string_view str, char delimiter) {
			std::vector<std::string> result;
			Split(result, str, delimiter);
			return result;
		}

		static inline std::vector<std::string> Split(std::string_view str, std::string_view delimiter = " \n\r\t") {
			std::vector<std::string> result;
			Split(result, str, delimiter);
			return result;
		}

		static inline void SplitV(std::vector<std::string_view>& result, std::string_view str, char delimiter) {
			result.reserve(result.size() + Count(str, delimiter));
			const char* start = str.data();
			const char* end = start + str.size();

			while (start < end) {
				const char* pos = (const char*)memchr(start, delimiter, end - start);
				if (!pos) {
					result.emplace_back(start, end - start);
					return;
				}
				result.emplace_back(start, pos - start);
				start = pos + 1;
			}
			result.emplace_back(start, 0);
		}

		static inline void SplitV(std::vector<std::string_view>& result, std::string_view str, std::string_view delimiter = " \n\r\t") {
			size_t start = 0, end;
			do {
				end = str.find_first_of(delimiter, start);
				result.emplace_back(str.substr(start, end - start));
				start = str.find_first_not_of(delimiter, end);
			} while (start != std::string::npos);
		}

		static inline std::vector<std::string_view> SplitV(std::string_view str, char delimiter) {
			std::vector<std::string_view> result;
			SplitV(result, str, delimiter);
			return result;
		}

		static inline std::vector<std::string_view> SplitV(std::string_view str, std::string_view delimiter = " \n\r\t") {
			std::vector<std::string_view> result;
			SplitV(result, str, delimiter);
			return result;
		}

		template<typename T>
		static inline std::string Join(const std::vector<T>& items, std::string_view delimiter = " ") {
			if (items.empty())
				return "";

			size_t total = 0;
			for (auto& s : items)
				total += s.size();
			total += delimiter.size() * (items.size() - 1);

			std::string result;
			result.reserve(total);
			result += items[0];
			for (size_t i = 1; i < items.size(); i++) {
				result += delimiter;
				result += items[i];
			}
			return result;
		}

		static bool IsInteger(std::string_view s) {
			if (s.empty())
				return false;

			s = TrimV(s);
			size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
			if (i == s.size())
				return false;

			for (; i < s.size(); i++)
				if (!std::isdigit((unsigned char)s[i]))
					return false;
			return true;
		}

		static inline std::string LTrim(std::string_view s, std::string_view delimiter = " \n\r\t") {
			size_t start = s.find_first_not_of(delimiter);
			return start == std::string::npos ? "" : std::string(s.substr(start));
		}

		static inline std::string_view LTrimV(std::string_view s, std::string_view delimiter = " \n\r\t") {
			size_t start = s.find_first_not_of(delimiter);
			return start == std::string::npos ? s.substr(0, 0) : s.substr(start);
		}

		static inline void LTrimSelf(std::string& s, std::string_view delimiter = " \n\r\t") {
			s.erase(0, s.find_first_not_of(delimiter));
		}

		static inline std::string RTrim(std::string_view s, std::string_view delimiter = " \n\r\t") {
			size_t end = s.find_last_not_of(delimiter);
			return std::string(s.substr(0, end == std::string::npos ? 0 : end + 1));
		}

		static inline std::string_view RTrimV(std::string_view s, std::string_view delimiter = " \n\r\t") {
			size_t end = s.find_last_not_of(delimiter);
			return s.substr(0, end == std::string::npos ? 0 : end + 1);
		}

		static inline void RTrimSelf(std::string& s, std::string_view delimiter = " \n\r\t") {
			size_t end = s.find_last_not_of(delimiter);
			s.erase(end == std::string::npos ? 0 : end + 1, std::string::npos);
		}

		static inline std::string Trim(std::string_view s, std::string_view delimiter = " \n\r\t") {
			size_t start = s.find_first_not_of(delimiter);
			if (start == std::string::npos)
				return "";
			size_t end = s.find_last_not_of(delimiter);
			return std::string(s.substr(start, end - start + 1));
		}

		static inline std::string_view TrimV(std::string_view s, std::string_view delimiter = " \n\r\t") {
			size_t start = s.find_first_not_of(delimiter);
			if (start == std::string::npos)
				return s.substr(0, 0);
			size_t end = s.find_last_not_of(delimiter);
			return s.substr(start, end - start + 1);
		}

		static inline void TrimSelf(std::string& s, std::string_view delimiter = " \n\r\t") {
			LTrimSelf(s, delimiter);
			RTrimSelf(s, delimiter);
		}

		static std::string Lower(std::string_view s) {
			std::string result = std::string(s);
			for (auto& c : result)
				c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			return result;
		}

		static void LowerSelf(std::string& s) {
			for (auto& c : s)
				c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		}

		static std::string Upper(std::string_view s) {
			std::string result = std::string(s);
			for (auto& c : result)
				c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
			return result;
		}

		static void UpperSelf(std::string& s) {
			for (auto& c : s)
				c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
		}

		static bool StartsWith(std::string_view s, std::string_view prefix) {
			return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
		}

		static bool EndsWith(std::string_view s, std::string_view suffix) {
			return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
		}

		static bool Contains(std::string_view s, std::string_view target) {
			return s.find(target) != std::string_view::npos;
		}

		static void Replace(std::string& str, std::string_view from, std::string_view to) {
			if (from.empty())
				return;
			size_t pos = 0;
			while ((pos = str.find(from, pos)) != std::string::npos) {
				str.replace(pos, from.size(), to);
				pos += to.size();
			}
		}
		
		static inline std::string PadLeft(std::string_view s, size_t width, char pad = ' ') {
			if (s.size() >= width)
				return std::string(s);
			std::string result;
			result.reserve(width);
			result.append(width - s.size(), pad);
			result.append(s);
			return result;
		}

		static inline std::string PadRight(std::string_view s, size_t width, char pad = ' ') {
			if (s.size() >= width)
				return std::string(s);
			std::string result;
			result.reserve(width);
			result.append(s);
			result.append(width - s.size(), pad);
			return result;
		}

		static inline void PadLeftSelf(std::string& s, size_t width, char pad = ' ') {
			if (s.size() >= width)
				return;
			s.insert(0, width - s.size(), pad);
		}

		static inline void PadRightInPlace(std::string& s, size_t width, char pad = ' ') {
			if (s.size() >= width)
				return;
			s.append(width - s.size(), pad);
		}

		//static inline std::string ToStrMinDigits(int value, int count) {
		//	std::string num = std::to_string(value);
		//	count -= static_cast<int>(num.length());
		//	return std::string(count, '0') + num;
		//}

		static std::wstring to_wstring(std::string_view string) {
			return std::wstring(string.begin(), string.end());
		}
		
		static std::string to_string(const std::wstring& string) {
			return std::string(string.begin(), string.end());
		}
	};
}