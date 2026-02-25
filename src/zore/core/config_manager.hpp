#pragma once
#include <string>
#include <unordered_map>

namespace zore::Config {

	//========================================================================
	//	Config Manager Class
	//========================================================================

	class Manager {
	public:
		Manager() = default;
		Manager(const std::string& filename);
		~Manager() = default;

		void Load(const std::string& filename);
		void Save(const std::string& filename);
		void Save();

		const std::string& GetString(const std::string& key, const std::string& default_value = "");
		int GetInt(const std::string& key, int default_value = 0);
		float GetFloat(const std::string& key, float default_value = 0.f);
		bool GetBool(const std::string& key, bool default_value = false);

		void Set(const std::string& key, const std::string& value);
		void Set(const std::string& key, int value);
		void Set(const std::string& key, float value);
		void Set(const std::string& key, bool value);

	private:
		std::string m_filename;
		std::unordered_map<std::string, std::string> m_entries;
		bool m_should_save;
	};
}