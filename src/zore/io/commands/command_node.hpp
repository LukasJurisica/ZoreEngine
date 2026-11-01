#pragma once

#include "zore/io/commands/command_context.hpp"
#include "zore/io/commands/argument_parser.hpp"
#include "zore/utils/memory.hpp"
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

	public:
		Node(std::string_view name, Node* parent, Type type, bool greedy);
		Node(const Node&) = delete;
		Node& operator=(const Node&) = delete;
		Node(Node&& other) = delete;
		Node& operator=(Node&& other) = delete;
		~Node() = default;

		Node& Literal(std::string_view name);
		Node& Argument(std::string_view name, bool greedy = false);
		Node& Unregister(std::string_view name);
		Node& Description(std::string description);
		Node& Executes(Action action);
		std::string Path() const;
		std::string Target() const;

		Node* Insert(std::string_view name, Type type, bool greedy = false);
		Node* GetChild(std::string_view name);

	private:
		bool HasArgument() const;

	private:
		std::string m_name;
		std::string m_description = "This command has no description";
		std::vector<unique_pointer<Node>> m_children;
		Node* m_parent;
		Action m_action = nullptr;
		Type m_type;
		bool m_greedy;
	};
}