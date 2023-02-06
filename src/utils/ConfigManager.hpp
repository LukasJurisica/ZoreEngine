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
		int Set(const std::string& key, int value);
		int Toggle(const std::string& key);

	//private:
	//	enum class ConfigValueType { INT, FLOAT };

	//	union ConfigData {
	//		int i;
	//		float f;
	//	};

	//	struct ConfigValue {
	//		ConfigValue(const std::string& value);
	//		std::string ToString();

	//		ConfigValueType type;
	//		ConfigData data;
	//	};

	private:
		std::string filename;
		std::unordered_map<std::string, int> entries;
		bool shouldSave;
	};
}