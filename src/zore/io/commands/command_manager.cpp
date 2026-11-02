#include "zore/io/commands/command_manager.hpp"
#include "zore/debug.hpp"

namespace zore::Command {

	static Node s_root("root_node", nullptr, Node::Type::ROOT, false);

	//========================================================================
	//	Command Manager
	//========================================================================

	void Manager::Init() {
		Clear();
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

	Result Manager::Evaluate(std::string_view command) {
		StringReader reader(command);
		Context context;
		Node* node = Traverse(&s_root, reader, context);
		if (node == nullptr)
			return { std::format("Unknown command or invalid arguments: '{}'. Type '/help' for available commands.", command), false };
		else if (!reader.Remaining().empty())
			return { std::format("Unexpected input after '/{}': '{}'.", node->Path(), reader.Remaining()), false };
		else if (node->m_action) {
			try {
				node->m_action(context);
				return { context.Response(), true };
			}
			catch (const std::exception& e) {
				return { std::string(e.what()), false };
			}
		}
		else if (node->m_children.size() > 0)
			return { std::format("Incomplete command. Type '/help {}' for usage.", node->Path()), false };
		else
			return { "Command is incomplete: it has no action.", false };
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
					throw ParseException("Unknown command path: " + std::string(command));
					return;
				}

				context.Response() = std::format("/{}: {}", selected->Path(), selected->m_description);
				context.Response() += "\nUsage:";

				bool found_action = false;
				auto print_usage = [&](auto&& self, const Node& node) -> void {
					if (node.m_action) {
						context.Response() += "\n  /" + node.Path();
						found_action = true;
					}

					for (const auto& child : node.m_children)
						self(self, *child);
					};

				print_usage(print_usage, *selected);
				if (!found_action)
					context.Response() += "  \nNo executable forms are registered.";
			}
			catch (const ParseException& error) {
				throw std::runtime_error("Invalid help query: " + std::string(error.what()));
				return;
			}
		}
		else if (s_root.m_children.empty()) {
			context.Response() = "No commands are registered.";
		}
		else {
			context.Response() = "--- Available Commands ---";
			for (auto& n : s_root.m_children)
				context.Response() += "\n" + n->Target() + ": " + n->m_description;
			context.Response() += "\nUse /help <command> for usage.";
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