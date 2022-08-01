#pragma once
#include <string>
#include <unordered_map>

namespace zore {
	class ConfigGroup {
	public:
		ConfigGroup(const std::string& filename);
		~ConfigGroup() = default;

		void Load();
		void Save();
		int Get(const std::string& key, int defaultValue = 0);
		void Set(const std::string& key, int value);

	private:
		std::string filename;
		std::unordered_map<std::string, int> entries;
		bool shouldSave;
	};
}