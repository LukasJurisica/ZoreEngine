#include "zore/core/config_manager.hpp"
#include "zore/core/file_manager.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"

namespace zore::Config {

	zore::string_unordered_map<Manager> s_managers;

	Manager::Manager(std::string_view filename) {
		m_filename = filename;
		Reload();
	}

	Manager& Manager::Load(std::string_view filename) {
		auto iter = s_managers.find(filename);
		if (iter != s_managers.end())
			return iter->second;
		return s_managers.emplace(filename, filename).first->second;
	}

	void Manager::Reload() {
		m_entries.clear();
		m_should_save = false;

		File::Manager::EnsureDir("config");
		File file("config/" + m_filename + ".cfg");
		for (const std::string& line : file) {
			size_t index = line.find("=");
			std::string key = String::Trim(line.substr(0, index));
			std::string value = String::Trim(line.substr(index + 1));
			m_entries[key] = value;
		}
	}

	void Manager::Save() {
		if (m_should_save) {
			File file("config/" + m_filename + ".cfg");
			std::string output;
			for (auto& iter : m_entries)
				output += iter.first + "=" + iter.second + "\n";
			file.Write(output);
			m_should_save = false;
		}
	}
}