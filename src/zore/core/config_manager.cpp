#include "zore/core/config_manager.hpp"
#include "zore/core/file_manager.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"
#include <vector>

namespace zore::Config {

	//========================================================================
	//	Config Manager Class
	//========================================================================

	Manager::Manager(const std::string& filename) : m_should_save(false) {
		Load(filename);
	}

	void Manager::Load(const std::string& filename) {
		FileManager::EnsureDir("config");
		m_filename = "config/" + filename + ".cfg";
		std::vector<std::string> content;
		FileManager::ReadLines(content, m_filename, false, false);
		for (std::string& line : content) {
			String::TrimInPlace(line);
			size_t index = line.find("=");
			std::string key = String::RTrim(line.substr(0, index));
			m_entries[key] = line.substr(index + 1);
		}
	}

	void Manager::Save(const std::string& filename) {
		m_filename = "config/" + filename + ".cfg";
		Save();
	}

	void Manager::Save() {
		if (m_should_save) {
			std::string output;
			for (auto& iter : m_entries)
				output += iter.first + "=" + iter.second + "\n";
			FileManager::WriteContent(output, m_filename, true);
			m_should_save = false;
		}
	}

	const std::string& Manager::GetString(const std::string& key, const std::string& default_value) {
		auto iter = m_entries.find(key);
		if (iter == m_entries.end()) {
			Set(key, default_value);
			return default_value;
		}
		return iter->second;
	}

	int Manager::GetInt(const std::string& key, int default_value) {
		try {
			return std::stoi(GetString(key, std::to_string(default_value)));
		}
		catch (const std::exception& e) {
			Set(key, std::to_string(default_value));
		}
		return default_value;
	}

	float Manager::GetFloat(const std::string& key, float default_value) {
		try {
			return std::stof(GetString(key, std::to_string(default_value)));
		}
		catch (const std::exception& e) {
			Set(key, std::to_string(default_value));
		}
		return default_value;
	}

	bool Manager::GetBool(const std::string& key, bool default_value) {
		const std::string& value = GetString(key, default_value ? "true" : "false");
		if (value == "true")
			return true;
		else if (value == "false")
			return false;
		else
			Set(key, default_value);
		return default_value;
	}

	void Manager::Set(const std::string& key, const std::string& value) {
		m_entries[key] = value;
		m_should_save = true;
	}

	void Manager::Set(const std::string& key, int value) {
		Set(key, std::to_string(value));
	}

	void Manager::Set(const std::string& key, float value) {
		Set(key, std::to_string(value));
	}

	void Manager::Set(const std::string& key, bool value) {
		const std::string possible_values[] = { "false", "true" };
		Set(key, possible_values[static_cast<int>(value)]);
	}
}