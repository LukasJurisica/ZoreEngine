#pragma once

#include <stdexcept>

namespace zore::Command {

	//========================================================================
	//	Parse Exception
	//========================================================================

	class ParseException : public std::runtime_error {
	public:
		ParseException(const std::string& msg) : std::runtime_error(msg) {};
		~ParseException() = default;
	};
}