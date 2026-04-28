#pragma once

#include <string>
#include <vector>

//========================================================================
//	Command Utility
//========================================================================

namespace zore {

	class Command {
	public:
		static void Register(std::string_view command, void (*func)(std::string_view, const std::vector<std::string_view>&));
		static void Unregister(std::string_view command);
		static void UnregisterAll();
		static void Process(std::string_view command);

		static void Help(std::string_view, const std::vector<std::string_view>& args);
	};
}