#pragma once

#include "string_reader.hpp"
#include "zore/utils/concepts.hpp"

namespace zore::Command {

	//========================================================================
	//	Parse Exception
	//========================================================================

	class ParseException : public std::runtime_error {
	public:
		ParseException(const std::string& msg, size_t position) : std::runtime_error(msg), m_position(position) {};
		~ParseException() = default;

	private:
		size_t m_position;
	};

	//========================================================================
	//	Greey String struct
	//========================================================================

	struct greedy_string {
		std::string m_content;
	};

	//========================================================================
	//	Argument Parser
	//========================================================================

	template <typename T>
	struct ArgumentParser;

	template <typename T>
	concept Parsable = std::copy_constructible<T> && requires(StringReader& reader) {
		{ ArgumentParser<T>::Parse(reader) } -> std::same_as<T>;
	};
}

#include "argument_parser.inl"