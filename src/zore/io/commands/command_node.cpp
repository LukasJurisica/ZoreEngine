#include "zore/io/commands/command_node.hpp"
#include "zore/debug.hpp"

namespace zore::Command {

	//========================================================================
	//	Command Node
	//========================================================================

	Node::Node(std::string_view name, Node* parent, Type type, Parser parser) : m_parser(parser), m_name(name), m_parent(parent), m_type(type) {
		ENSURE(type != Type::ARGUMENT || parser != nullptr, "Attempted to initialize argument node without a parser.");
		ENSURE(!name.empty(), "Cannot create a command node with an empty name.");
		for (unsigned char c : name)
			ENSURE(!(c <= 32 || c == 127 || c == '/' || c == '<' || c == '>' || c == '\'' || c == '"'), "Node names cannot contain whitespace, control characters, slashes, quotes, or angle brackets");
		ENSURE(type != Type::ROOT || name != "help", "The root literal 'help' is reserved");
		uint32_t depth = 0;
		for (Node* parent = this; parent; parent = parent->m_parent) {
			ENSURE(++depth <= 16, "Command paths cannot exceed 64 nodes");
			ENSURE(type != Type::ARGUMENT || parent->m_type != Type::ARGUMENT || parent->m_name != name, "Argument name already exists in this path: " + std::string(name));
		}
	}

	Node& Node::Literal(std::string_view name) {
		return *Insert(name, Type::LITERAL);
	}

	Node& Node::Description(std::string description) {
		m_description = description;
		return *this;
	}

	Node& Node::Executes(Action action) {
		m_action = action;
		return *this;
	}

	Node* Node::Insert(std::string_view name, Type type, Parser parser) {
		for (zore::unique_pointer<Node>& p : m_children) {
			if (p->m_name == name) {
				Node* node = p.get();
				if (node->m_type != type)
					Logger::Error("Bad Stuff");
				return node;
			}
		}

		Node* node = new Node(name, this, type, parser);
		if (type == Type::LITERAL)
			m_children.insert(m_children.begin() + literal_children_count++, zore::unique_pointer(node));
		else
			m_children.push_back(zore::unique_pointer(node));
		return node;
	}
}