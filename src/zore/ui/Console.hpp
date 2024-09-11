#pragma once

#include <string>
#include <vector>

namespace zore {

	class Console {
	public:
		enum class LogLevel { LOG, INFO, WARN, ERR, CMD };

	public:
		static void Draw();
		static void RegisterCommand(const std::string& command, void (*func)(const std::vector<std::string>&));
		static void UnregisterCommand(const std::string& command);
		static void UnregisterAllCommands();
		static void Print(const std::string& message, LogLevel level = LogLevel::LOG);
		static void Clear();
		static void Dump();

		static void Help(const std::vector<std::string>& args);

	private:
		static void ProcessCommand();
	};
}