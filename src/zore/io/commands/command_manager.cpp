#include "zore/io/commands/command_manager.hpp"
#include "zore/debug.hpp"

namespace zore::Command {

	static Node s_root("root_node", nullptr, Node::Type::ROOT, false);

	//========================================================================
	//	Command Manager
	//========================================================================

	void Manager::Init() {
		Register("help")
			.Executes(Manager::Help)
			.Description("Provides a list of all available commands")
			.Argument("command", true)
			.Executes(Manager::Help);
	}

	Node& Manager::Register(std::string_view name) {
		return s_root.Literal(name);
	}

	void Manager::Unregister(std::string_view name) {
		s_root.Unregister(name);
	}

	void Manager::Evaluate(std::string_view command) {
		StringReader reader(command);
		Context context;
		Node* result = Traverse(&s_root, reader, context);
		if (result == nullptr)
			Logger::Error(std::format("Unknown command or invalid arguments: '{}'. Type '/help' for available commands.", command));
		else if (!reader.Remaining().empty())
			Logger::Error(std::format("Unexpected input after '/{}': '{}'.", result->Path(), reader.Remaining()));
		else if (result->m_action)
			result->m_action(context);
		else if (result->m_children.size() > 0)
			Logger::Error(std::format("Incomplete command. Type '/help {}' for usage.", result->Path()));
		else
			Logger::Warn("Command is incomplete: it has no action.");
	}

	void Manager::Clear() {
		s_root.m_children.clear();
	}

	void Manager::Help(Context& context) {
		if (context.Has("command")) {
			std::string command(context.Get<std::string_view>("command"));
			if (command.front() != '/')
				command.insert(command.begin(), '/');

			StringReader reader(command);
			Context lookup_context;

			try {
				Node* selected = Traverse(&s_root, reader, lookup_context);

				// Traverse stops at leaves, so reject unconsumed input too.
				if (!selected || !reader.Remaining().empty()) {
					Logger::Error("Unknown command path: " + std::string(command));
					return;
				}

				Logger::Log(std::format("/{}: {}", selected->Path(), selected->m_description));
				Logger::Log("Usage:");

				bool found_action = false;
				auto print_usage = [&](auto&& self, const Node& node) -> void {
					if (node.m_action) {
						Logger::Log("  /" + node.Path());
						found_action = true;
					}

					for (const auto& child : node.m_children)
						self(self, *child);
					};

				print_usage(print_usage, *selected);
				if (!found_action)
					Logger::Log("  No executable forms are registered.");
			}
			catch (const ParseException& error) {
				Logger::Error(std::string("Invalid help query: ") + error.what());
				return;
			}
		}
		else if (s_root.m_children.empty()) {
			Logger::Log("No commands are registered.");
		}
		else {
			Logger::Log("--- Available Commands ---");
			for (auto& n : s_root.m_children)
				Logger::Log(n->Target() + ": " + n->m_description);
			Logger::Log("Use /help <command> for usage.");
		}
	}

	Node* Manager::Traverse(Node* node, StringReader& reader, Context& context) {
		std::string_view remaining = reader.Remaining();
		if (node->m_children.empty() || remaining.empty())
			return node;
		std::string_view token = reader.Read();

		int literal_node_count = node->m_children.size() - (node->HasArgument() ? 1 : 0);
		for (int i = 0; i < literal_node_count; i++)
			if (token == node->m_children[i]->Target())
				return Traverse(node->m_children[i].get(), reader, context);
		if (node->HasArgument()) {
			Node* next = node->m_children.back().get();
			if (next->m_greedy) {
				context.Set(next->m_name, remaining);
				reader.Close();
			}
			else
				context.Set(next->m_name, token);
			return Traverse(next, reader, context);
		}
		return nullptr;
	}
}