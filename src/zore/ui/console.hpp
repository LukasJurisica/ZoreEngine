#pragma once

#include <string>
#include <vector>

namespace zore {

	class Console {
	public:
		enum class LogLevel { LOG, INFO, WARN, ERR, CMD };

	public:
		static void Draw();
		static void RegisterCommand(const std::string& command, bool (*func)(const std::vector<std::string>&));
		static void UnregisterCommand(const std::string& command);
		static void UnregisterAllCommands();
		static bool ProcessCommand(const std::string& s, bool print = true);
		static void Print(const std::string& message, LogLevel level = LogLevel::LOG);
		static void Clear();
		static void Dump();

		static bool Help(const std::vector<std::string>& args);
	};
}