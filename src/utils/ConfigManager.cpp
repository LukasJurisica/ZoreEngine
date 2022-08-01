#include "utils/ConfigManager.hpp"
#include "utils/FileManager.hpp"
#include "utils/StringUtils.hpp"
#include "debug/Debug.hpp"
#include <vector>

namespace zore {

	ConfigGroup::ConfigGroup(const std::string& filename) : filename("config/" + filename + ".cfg"), shouldSave(false) {}

	void ConfigGroup::Load() {
		std::vector<std::string> content;
		FileManager::ReadLines(content, filename, false, false);
		for (std::string& line : content) {
			StringUtils::TrimInPlace(line);
			size_t index = line.find("=");
			std::string key = StringUtils::RTrim(line.substr(0, index));
			entries[key] = std::stoi(line.substr(index + 1));
		}
	}

	void ConfigGroup::Save() {
		if (shouldSave) {
			std::string output;
			auto iter = entries.begin();
			while (iter != entries.end()) {
				output += iter->first + "=" + std::to_string(iter->second) + "\n";
				iter++;
			}
			FileManager::WriteContent(output, filename, true);
			shouldSave = false;
		}
	}

	int ConfigGroup::Get(const std::string& key, int defaultValue) {
		Logger::Log(key);
		auto iter = entries.find(key);
		if (iter == entries.end()) {
			entries.insert({ key, defaultValue });
			shouldSave = true;
			return defaultValue;
		}
		return iter->second;
	}

	void ConfigGroup::Set(const std::string& key, int value) {
		entries[key] = value;
		shouldSave = true;
	}

}