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
		int Get(const std::string& key, int default_value = 0);
		int Set(const std::string& key, int value);

	private:
		std::string m_filename;
		std::unordered_map<std::string, std::string> m_entries;
		bool m_should_save;
	};
}