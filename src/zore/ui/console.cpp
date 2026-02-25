#include "zore/ui/console.hpp"
#include "zore/core/command.hpp"
#include "zore/core/file_manager.hpp"
#include "zore/utils/time.hpp"
#include "zore/debug.hpp"
#include <imgui.h>
#include <fstream>
#include <ctime>

namespace zore {

	struct LogEntry {
		std::string text;
		Console::LogLevel level;
	};

	static char s_buffer[256];
	static std::vector<LogEntry> s_log_entries;
	static std::vector<uint32_t> s_history;
	static int32_t s_history_pos = 1;

	static bool s_scroll_to_bottom = false;
	static bool s_retain_focus = false;
	static bool s_log_level_filter[] = { true, true, true, true, true };

	static int TextEditCallback(ImGuiInputTextCallbackData* data) {
		switch (data->EventFlag) {
		case ImGuiInputTextFlags_CallbackCompletion:
			// Not implemented yet
			break;
		case ImGuiInputTextFlags_CallbackHistory:
			const int prev_history_pos = s_history_pos;
			if (data->EventKey == ImGuiKey_UpArrow && s_history_pos > 0)
				s_history_pos--;
			else if (data->EventKey == ImGuiKey_DownArrow && s_history_pos < s_history.size())
				s_history_pos++;

			if (prev_history_pos != s_history_pos) {
				const std::string& history_str = s_history_pos == s_history.size() ? "" : s_log_entries[s_history[s_history_pos]].text;
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str.c_str() + 2);
			}
			break;
		}
		return 0;
	}

	void Console::Draw() {
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
		if (ImGui::Begin("Console", nullptr, window_flags)) {

			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("Filter")) {
					ImGui::Checkbox("Logs", &s_log_level_filter[0]);
					ImGui::Checkbox("Info", &s_log_level_filter[1]);
					ImGui::Checkbox("Warnings", &s_log_level_filter[2]);
					ImGui::Checkbox("Errors", &s_log_level_filter[3]);
					ImGui::Checkbox("Commands", &s_log_level_filter[4]);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Content")) {
					if (ImGui::MenuItem("Clear"))
						Clear();
					if (ImGui::MenuItem("Dump"))
						Dump();
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::PushItemWidth(-1);
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
				for (const LogEntry& item : s_log_entries) {
					static constexpr ImVec4 log_colours[] = {
						ImVec4(0.8f, 0.8f, 0.8f, 1.0f), // Log
						ImVec4(0.2f, 0.5f, 0.8f, 1.0f), // Info
						ImVec4(0.8f, 0.6f, 0.0f, 1.0f), // Warning
						ImVec4(0.9f, 0.1f, 0.1f, 1.0f), // Error
						ImVec4(0.1f, 0.8f, 0.5f, 1.0f)  // Command
					};
					if (s_log_level_filter[static_cast<uint32_t>(item.level)]) {
						ImGui::PushStyleColor(ImGuiCol_Text, log_colours[static_cast<uint32_t>(item.level)]);
						ImGui::TextUnformatted(item.text.c_str());
						ImGui::PopStyleColor();
					}
				}
				// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
				if (s_scroll_to_bottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
					ImGui::SetScrollHereY(1.0f);
				s_scroll_to_bottom = false;
				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
			ImGui::Separator();

			// Retain focus if they just input a command
			if (s_retain_focus) {
				ImGui::SetKeyboardFocusHere(0);
				s_retain_focus = false;
			}
			ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
			if (ImGui::InputText("Console", s_buffer, sizeof(s_buffer), input_text_flags, &TextEditCallback)) {
				std::string command = std::string(s_buffer);
				memset(s_buffer, 0, 256);
				Console::Print("> " + command, Console::LogLevel::CMD);
				Command::Process(command);
				if (s_history.size() == 0 || s_log_entries[s_history.back()].text != "> " + command)
					s_history.push_back(s_log_entries.size() - 1);
				s_history_pos = s_history.size();
				s_scroll_to_bottom = true;
				s_retain_focus = true;
			}
		}
		ImGui::End();
	}

	void Console::Print(const std::string& message, LogLevel level) {
		s_log_entries.push_back({ message, level });
	}

	void Console::Clear() {
		s_log_entries.clear();
	}

	void Console::Dump() {
		time_t timestamp = time(NULL);
		struct tm datetime = *localtime(&timestamp);

		FileManager::EnsureDir("logs");
		char buffer[20];
		strftime(buffer, 20, "%Y-%m-%d_%I-%M-%S", &datetime);
		std::string filename = "logs/crashlog_" + std::string(buffer) + ".log";

		static const std::string log_names[] = { "Info", "Warn", "Error" };

		std::ofstream file(filename);
		if (file.is_open()) {
			for (const LogEntry& item : s_log_entries) {
				if (item.level == LogLevel::INFO || item.level == LogLevel::WARN || item.level == LogLevel::ERR)
					file << "[" << log_names[static_cast<uint32_t>(item.level) - 1] << "] " << item.text << std::endl;
			}
		}
	}
}