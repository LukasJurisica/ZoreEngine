#pragma once
#include "zore/io/commands/command_node.hpp"

namespace zore::Command {

	//========================================================================
	//	Command Result
	//========================================================================

	struct Result {
		std::string message;
		bool success;
	};

	//========================================================================
	//	Command Manager
	//========================================================================
	
	class Manager {
	public:
		static void Init();
		static Node& Register(std::string_view name);
		static void Unregister(std::string_view name);
		static Result Evaluate(std::string_view command);
		static void Clear();

	private:
		static void Help(Context& context);
		static Node* Traverse(Node* node, StringReader& reader, Context& context);
	};
}