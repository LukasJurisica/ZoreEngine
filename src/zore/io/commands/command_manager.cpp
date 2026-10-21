#include "zore/io/commands/command_manager.hpp"

namespace zore::Command {

	static Node s_root("root_node", nullptr, Node::Type::ROOT);

	Node& Manager::Register(std::string_view name) {
		return s_root.Literal(name);
	}

	void Manager::Unregister(std::string_view name) {

	}

	void Manager::Clear() {
		s_root.m_children.clear();
	}
}