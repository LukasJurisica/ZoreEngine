#include "zore/core/FileManager.hpp"
#include "zore/utils/ConfigManager.hpp"
#include "zore/utils/StringUtils.hpp"
#include "zore/Debug.hpp"

#include <vector>

namespace zore {

	ConfigGroup::ConfigGroup(const std::string& filename) : m_filename("config/" + filename + ".cfg"), m_should_save(false) {}

	//ConfigGroup::ConfigValue::ConfigValue(const std::string& value) {
	//	if (value.find(".") != std::string::npos) { // Floating Point
	//		type = ConfigValueType::FLOAT;
	//		data.f = std::stof(value);
	//	}
	//	else { // Integer
	//		type = ConfigValueType::INT;
	//		data.i = std::stoi(value);
	//	}
	//}

	//std::string ConfigGroup::ConfigValue::ToString() {

	//}

	void ConfigGroup::Load() {
		std::vector<std::string> content;
		FileManager::ReadLines(content, m_filename, false, false);
		for (std::string& line : content) {
			StringUtils::TrimInPlace(line);
			size_t index = line.find("=");
			std::string key = StringUtils::RTrim(line.substr(0, index));
			m_entries[key] = std::stoi(line.substr(index + 1));
		}
	}

	void ConfigGroup::Save() {
		if (m_should_save) {
			std::string output;
			for (auto& iter : m_entries)
				output += iter.first + "=" + std::to_string(iter.second) + "\n";
			FileManager::WriteContent(output, m_filename, true);
			m_should_save = false;
		}
	}

	int ConfigGroup::Get(const std::string& key, int default_value) {
		auto iter = m_entries.find(key);
		if (iter == m_entries.end()) {
			m_entries.insert({ key, default_value });
			m_should_save = true;
			return default_value;
		}
		return iter->second;
	}

	int ConfigGroup::Set(const std::string& key, int value) {
		m_entries[key] = value;
		m_should_save = true;
		return value;
	}
}