#include "zore/core/command.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug/logger.hpp"
#include <unordered_map>

namespace zore {

	static std::unordered_map<std::string, void (*)(const std::vector<std::string>&)> s_commands;
	static std::unordered_map<std::string, void (*)(const std::string&, const std::vector<std::string>&)> s_commands_full;

	void Command::Register(const std::string& command, void (*func)(const std::vector<std::string>&)) {
		if (auto iter = s_commands_full.find(command); iter != s_commands_full.end())
			s_commands_full.erase(iter);
		s_commands[String::Lower(command)] = func;
	}


	void Command::Register(const std::string& command, void (*func)(const std::string&, const std::vector<std::string>&)) {
		if (auto iter = s_commands.find(command); iter != s_commands.end())
			s_commands.erase(iter);
		s_commands_full[String::Lower(command)] = func;
	}

	void Command::Unregister(const std::string& command) {
		if (auto iter = s_commands.find(command); iter != s_commands.end())
			s_commands.erase(iter);
		if (auto iter = s_commands_full.find(command); iter != s_commands_full.end())
			s_commands_full.erase(iter);
	}

	void Command::UnregisterAll() {
		s_commands.clear();
		s_commands_full.clear();
	}

	void Command::Process(const std::string& command) {
		std::vector<std::string> args;
		String::Split(args, command, " ");
		String::LowerInPlace(args[0]);

		if (auto iter = s_commands_full.find(args[0]); iter != s_commands_full.end()) {
			try { iter->second(command, args); }
			catch (const std::exception& e) { Logger::Error("Error executing command:", std::string(e.what())); }
		}
		else if (auto iter = s_commands.find(args[0]); iter != s_commands.end()) {
			try { iter->second(args); }
			catch (const std::exception& e) { Logger::Error("Error executing command:", std::string(e.what())); }
		}
		else {
			Logger::Error("Command not found:", args[0]);
		}
	}

	void Command::Help(const std::vector<std::string>& args) {
		std::string result = "Commands:";
		for (const auto& command : s_commands)
			result += "\n- " + command.first;
		for (const auto& command : s_commands_full)
			result += "\n- " + command.first;
		Logger::Log(result);
	}
}