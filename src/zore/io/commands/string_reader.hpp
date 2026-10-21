#pragma once

#include <string_view>

namespace zore::Command {

	//========================================================================
	//	String Reader
	//========================================================================

	class StringReader {
	public:
		StringReader(std::string_view content, size_t position = 0);

		std::string_view Read();
		std::string_view Remaining();
		size_t GetPosition();

	private:
		inline bool CanRead() const;
		inline char Peek() const;
		inline void SkipWhitespace();

		void ReadQuoted(char quote);
		void ReadUnquoted();

	private:
		std::string_view m_content;
		size_t m_position = 0;
	};
}