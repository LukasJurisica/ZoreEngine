#include "zore/core/command.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug/logger.hpp"
#include <unordered_map>

namespace zore {

	static std::unordered_map<std::string, void (*)(const std::vector<std::string>&)> s_commands;

	void Command::Register(const std::string& command, void (*func)(const std::vector<std::string>&)) {
		s_commands[String::Lower(command)] = func;
	}

	void Command::Unregister(const std::string& command) {
		auto iter = s_commands.find(command);
		if (iter != s_commands.end())
			s_commands.erase(iter);
	}

	void Command::UnregisterAll() {
		s_commands.clear();
	}

	void Command::Process(const std::string& command) {
		std::vector<std::string> args;
		String::Split(args, command, " ");
		String::LowerInPlace(args[0]);

		auto iter = s_commands.find(args[0]);
		if (iter != s_commands.end()) {
			try {
				iter->second(args);
			}
			catch (const std::exception& e) {
				Logger::Error("Error executing command:", std::string(e.what()));
			}
		}
		else {
			Logger::Error("Command not found:", args[0]);
		}
	}

	void Command::Help(const std::vector<std::string>& args) {
		std::string result = "Commands:";
		for (const auto& command : s_commands)
			result += "\n- " + command.first;
		Logger::Log(result);
	}
}