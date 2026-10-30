#pragma once

#include "string_reader.hpp"
#include "zore/utils/concepts.hpp"

namespace zore::Command {

	//========================================================================
	//	Argument Parser
	//========================================================================

	template <typename T>
	struct ArgumentParser;

	template <typename T>
	concept Parsable = requires(std::string_view token) {
		{ ArgumentParser<T>::Parse(token) } -> std::same_as<T>;
	};
}

#include "argument_parser.inl"