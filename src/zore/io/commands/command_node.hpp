#pragma once

#include "zore/io/commands/command_context.hpp"
#include "zore/io/commands/argument_parser.hpp"
#include "zore/utils/memory.hpp"
//#include <functional>
#include <vector>

namespace zore::Command {

	//========================================================================
	//	Command Node
	//========================================================================
	
	class Node {
	public:
		friend class Manager;
		enum class Type { ROOT, LITERAL, ARGUMENT };

		using Action = void(*)(Context&);
		using Parser = std::any(*)(StringReader&);
		//using Action = std::function<void(Context&)>;
		//using Parser = std::function<std::any(StringReader&)>;

	public:
		Node(std::string_view name, Node* parent, Type type, Parser = nullptr);
		Node(const Node&) = delete;
		Node& operator=(const Node&) = delete;
		Node(Node&& other) = default;
		Node& operator=(Node&& other) = default;
		~Node() = default;

		Node& Literal(std::string_view name);
		template<Parsable T>
		Node& Argument(std::string_view name);
		Node& Description(std::string description);
		Node& Executes(Action action);

	private:
		Node* Insert(std::string_view name, Type type, Parser parser = nullptr);

	private:
		std::string m_name;
		std::string m_description;
		std::vector<unique_pointer<Node>> m_children;
		Node* m_parent;
		Action m_action = nullptr;
		Parser m_parser;
		uint32_t literal_children_count = 0;
		Type m_type;
	};
}

#include "zore/io/commands/command_node.inl"