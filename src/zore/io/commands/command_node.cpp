#include "zore/io/commands/command_node.hpp"
#include "zore/debug.hpp"

namespace zore::Command {

	//========================================================================
	//	Command Node
	//========================================================================

	Node::Node(std::string_view name, Node* parent, Type type, bool greedy) : m_name(name), m_parent(parent), m_type(type), m_greedy(greedy) {
		ENSURE(greedy == false || type == Type::ARGUMENT, "Only argument nodes can be greedy.");
		ENSURE(!name.empty(), "Cannot create a command node with an empty name.");
		for (unsigned char c : name)
			ENSURE(!(c <= 32 || c == 127 || c == '/' || c == '<' || c == '>' || c == '\'' || c == '"'), "Node names cannot contain whitespace, control characters, slashes, quotes, or angle brackets.");
		if (parent) {
			ENSURE(parent->m_greedy == false, "Cannot add a command node after a greedy argument.");
			ENSURE(!(type == Type::ARGUMENT && parent->HasArgument()), "Attempted to register 2 argument nodes at the same branch: " + parent->Path());
			uint32_t depth = 0;
			for (Node* parent = m_parent; parent; parent = parent->m_parent) {
				ENSURE(++depth <= 16, "Command paths cannot exceed 64 nodes");
				ENSURE(!(type == Type::ARGUMENT && parent->m_type == Type::ARGUMENT && parent->m_name == name), "Duplicate argument name in command path: " + std::string(name));
			}
		}
	}

	Node& Node::Literal(std::string_view name) {
		return *Insert(name, Type::LITERAL);
	}

	Node& Node::Argument(std::string_view name, bool greedy) {
		return *Insert(name, Type::ARGUMENT, greedy);
	}

	Node& Node::Unregister(std::string_view name) {
		for (int i = 0; i < m_children.size(); i++) {
			if (m_children[i]->m_name == name) {
				m_children.erase(m_children.begin() + i);
				break;
			}
		}
		return *this;
	}

	Node& Node::Description(std::string description) {
		m_description = description;
		return *this;
	}

	Node& Node::Executes(Action action) {
		m_action = action;
		return *this;
	}

	std::string Node::Path() const {
		std::string current = m_type == Type::ARGUMENT ? std::format("<{}{}>", m_name, m_greedy ? "..." : "") : m_name;

		if (!m_parent || m_parent->m_type == Type::ROOT)
			return current;

		return m_parent->Path() + " " + current;
	}
	
	std::string Node::Target() const {
		return (m_parent && m_parent->m_type == Type::ROOT) ? std::format("/{}", m_name) : m_name;
	}

	Node* Node::Insert(std::string_view name, Type type, bool greedy) {
		Node* node = GetChild(name);
		if (node) {
			ENSURE(node->m_type == type, "Attempted to create a command tree with matching nodes of different type: " + std::string(name));
			ENSURE(node->m_greedy == greedy, "Attempted to created a command tree with matching nodes of different greed: " + std::string(name));
			return node;
		}
		else {
			node = new Node(name, this, type, greedy);
			if (HasArgument())
				m_children.insert(m_children.end() - 1, zore::unique_pointer(node));
			else
				m_children.push_back(zore::unique_pointer(node));
			return node;
		}
	}

	Node* Node::GetChild(std::string_view name) {
		for (const zore::unique_pointer<Node>& node : m_children)
			if (node->m_name == name)
				return node.get();
		return nullptr;
	}

	bool Node::HasArgument() const {
		return m_children.size() > 0 && m_children.back()->m_type == Type::ARGUMENT;
	}
}