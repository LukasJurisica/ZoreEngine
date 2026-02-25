#pragma once

#include <string>
#include <vector>

//========================================================================
//	Command Utility
//========================================================================

namespace zore {

	class Command {
	public:
		static void Register(const std::string& command, void (*func)(const std::vector<std::string>&));
		static void Unregister(const std::string& command);
		static void UnregisterAll();
		static void Process(const std::string& command);

		static void Help(const std::vector<std::string>& args);
	};
}