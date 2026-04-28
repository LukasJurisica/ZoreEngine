#include "zore/core/command.hpp"
#include "zore/utils/string.hpp"
#include "zore/structures/string_unordered_map.hpp"
#include "zore/debug/logger.hpp"

namespace zore {
	
	static zore::string_unordered_map<void (*)(std::string_view command, const std::vector<std::string_view>&)> s_commands;

	void Command::Register(std::string_view command, void (*func)(std::string_view, const std::vector<std::string_view>&)) {
		s_commands[String::Lower(command)] = func;
	}

	void Command::Unregister(std::string_view command) {
		auto iter = s_commands.find(String::Lower(command));
		if (iter != s_commands.end())
			s_commands.erase(iter);
	}

	void Command::UnregisterAll() {
		s_commands.clear();
	}

	void Command::Process(std::string_view command) {
		std::vector<std::string_view> args;
		String::SplitV(args, command, " ");
		std::string command_lower = String::Lower(args[0]);

		if (auto iter = s_commands.find(command_lower); iter != s_commands.end())
			iter->second(command, args);
		else
			throw std::runtime_error("Command not found: " + std::string(args[0]));
	}

	void Command::Help(std::string_view, const std::vector<std::string_view>& args) {
		std::string result = "Commands:";
		for (const auto& command : s_commands)
			result += "\n- " + command.first;
		Logger::Log(result);
	}
}