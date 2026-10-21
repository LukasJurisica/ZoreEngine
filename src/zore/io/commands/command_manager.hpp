#pragma once
#include "zore/io/commands/command_node.hpp"

namespace zore::Command {

	//========================================================================
	//	Command Manager
	//========================================================================
	
	class Manager {
	public:
		static Node& Register(std::string_view name);
		static void Unregister(std::string_view name);
		static void Clear();

	private:

	};
}