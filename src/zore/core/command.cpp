#include "zore/core/command.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug/logger.hpp"
#include <unordered_map>

namespace zore {

	static std::unordered_map<std::string, void (*)(const std::string& command, const std::vector<std::string_view>&)> s_commands;

	void Command::Register(const std::string& command, void (*func)(const std::string&, const std::vector<std::string_view>&)) {
		s_commands[String::Lower(command)] = func;
	}

	void Command::Unregister(const std::string& command) {
		if (auto iter = s_commands.find(command); iter != s_commands.end())
			s_commands.erase(iter);
	}

	void Command::UnregisterAll() {
		s_commands.clear();
	}

	void Command::Process(const std::string& command) {
		std::vector<std::string_view> args;
		String::SplitV(args, command, " ");
		std::string command_lower = String::Lower(args[0]);

		if (auto iter = s_commands.find(command_lower); iter != s_commands.end())
			iter->second(command, args);
		else
			throw std::runtime_error("Command not found: " + std::string(args[0]));
	}

	void Command::Help(const std::string&, const std::vector<std::string_view>& args) {
		std::string result = "Commands:";
		for (const auto& command : s_commands)
			result += "\n- " + command.first;
		Logger::Log(result);
	}
}