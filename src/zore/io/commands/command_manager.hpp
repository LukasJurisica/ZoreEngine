#pragma once
#include "zore/io/commands/command_node.hpp"

namespace zore::Command {

	//========================================================================
	//	Command Manager
	//========================================================================
	
	class Manager {
	public:
		static void Init();
		static Node& Register(std::string_view name);
		static void Unregister(std::string_view name);
		static void Evaluate(std::string_view command);
		static void Clear();

	private:
		static void Help(Context& context);
		static Node* Traverse(Node* node, StringReader& reader, Context& context);
	};
}