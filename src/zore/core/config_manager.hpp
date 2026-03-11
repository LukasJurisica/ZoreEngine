#pragma once
#include "zore/structures/string_unordered_map.hpp"

namespace zore::Config {

	//========================================================================
	//	Config Manager Class
	//========================================================================

	class Manager {
	public:
		Manager(std::string_view filename);
		~Manager() = default;

		static Manager& Load(std::string_view filename);
		void Reload();
		void Save();

		template<typename T>
		void Set(std::string_view key, T value);
		template<>
		void Set(std::string_view key, bool value);
		template<>
		void Set(std::string_view key, std::string_view value);

		template<typename T>
		void Get(std::string_view key, T* value);
		template<>
		void Get(std::string_view key, bool* value);

		template<typename T>
		T Get(std::string_view key, T default_value);
		template<>
		std::string_view Get(std::string_view key, std::string_view default_value);

	private:
		std::string m_filename;
		zore::string_unordered_map<std::string> m_entries;
		bool m_should_save;
	};
}

#include "zore/core/config_manager.inl"