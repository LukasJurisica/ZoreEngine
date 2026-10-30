#include "zore/io/commands/string_reader.hpp"
#include "zore/io/commands/parse_exception.hpp"
#include "zore/debug.hpp"
#include <cctype>

namespace zore::Command {

	//========================================================================
	//	String Reader
	//========================================================================

	StringReader::StringReader(std::string_view content, size_t position) : m_content(content), m_position(position) {}

	std::string_view StringReader::Read() {
		SkipWhitespace();
		if (!CanRead())
			throw ParseException("Expected an argument");
		size_t start = m_position;
		const char first = Peek();
		bool quoted = first == '\'' || first == '"';
		quoted ? ReadQuoted(first) : ReadUnquoted();
		size_t offset = quoted ? 1 : 0;
		start += offset;
		m_token += 1;
		return m_content.substr(start, m_position - start - offset);
	}

	std::string_view StringReader::Remaining() {
		SkipWhitespace();
		return m_content.substr(m_position);
	}

	size_t StringReader::GetPosition() {
		return m_position;
	}

	void StringReader::Close() {
		m_position = m_content.size();
	}

	bool StringReader::CanRead() const {
		return m_position < m_content.size();
	}

	char StringReader::Peek() const {
		return m_content[m_position];
	}

	bool StringReader::IsWhitespace() const {
		return std::isspace(static_cast<unsigned char>(Peek()));
	}

	void StringReader::SkipWhitespace() {
		while (CanRead() && IsWhitespace())
			m_position++;
	}

	void StringReader::ReadQuoted(char quote) {
		m_position++;
		while (CanRead() && Peek() != quote)
			m_position++;
		if (!CanRead())
			throw ParseException("Unterminated quoted argument");
		m_position++;
	}

	void StringReader::ReadUnquoted() {
		while (CanRead() && IsWhitespace() == false)
			m_position++;
	}
}