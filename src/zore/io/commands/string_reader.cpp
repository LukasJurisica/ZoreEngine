#include "zore/io/commands/string_reader.hpp"
#include <cctype>
#include <zore/debug.hpp>

namespace zore::Command {

	//========================================================================
	//	String Reader
	//========================================================================

	StringReader::StringReader(std::string_view content, size_t position) : m_content(content), m_position(position) {}

	std::string_view StringReader::Read() {
		SkipWhitespace();
		size_t start = m_position;
		const char first = Peek();
		bool quoted = first == '\'' || first == '"';
		quoted ? ReadQuoted(first) : ReadUnquoted();
		start += quoted ? 1 : 0;
		return m_content.substr(start, m_position - start);
	}

	std::string_view StringReader::Remaining() {
		SkipWhitespace();
		return m_content.substr(m_position);
	}

	size_t StringReader::GetPosition() {
		return m_position;
	}

	bool StringReader::CanRead() const {
		return m_position < m_content.size();
	}

	char StringReader::Peek() const {
		return m_content[m_position];
	}

	void StringReader::SkipWhitespace() {
		while (CanRead() && std::isspace(Peek()))
			m_position++;
	}

	void StringReader::ReadQuoted(char quote) {
		m_position++;
		while (CanRead() && Peek() != quote)
			m_position++;
	}

	void StringReader::ReadUnquoted() {
		while (CanRead() && std::isspace(Peek()) == false)
			m_position++;
	}
}