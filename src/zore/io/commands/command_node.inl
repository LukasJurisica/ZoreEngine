#pragma once

#ifdef __INTELLISENSE__
#include "zore/io/commands/command_node.hpp"
#endif

namespace zore::Command {

    //========================================================================
    //	Command Node
    //========================================================================

	template <Parsable T>
	std::any Parse(StringReader& reader) {
		return ArgumentParser<T>::Parse(reader);
	}

	template <Parsable T>
	Node& Node::Argument(std::string_view name) {
		return *Insert(name, Type::ARGUMENT, Parse<T>);
	}
}