#pragma once

#ifdef __INTELLISENSE__
#include "zore/core/config_manager.hpp"
#endif
#include "zore/utils/string.hpp"

namespace zore::Config {

	template<typename T>
	void Manager::Set(std::string_view key, T value) {
		m_entries[key] = std::to_string(value);
	}

	template<>
	void Manager::Set(std::string_view key, bool value) {
		const std::string_view possible_values[] = { "false", "true" };
		Set(key, possible_values[value ? 1 : 0]);
	}

	template<typename T>
	void Manager::Get(std::string_view key, T* value) {
		auto iter = m_entries.find(key);
		if (iter != m_entries.end())
			String::Parse<T>(iter->second, value);
		else
			Set(key, *value);
	}

	template<>
	void Manager::Get(std::string_view key, bool* value) {
		auto iter = m_entries.find(key);
		if (iter != m_entries.end())
			*value = String::Lower(iter->second) == "true";
		else
			Set(key, *value);
	}

	template<typename T>
	T Manager::Get(std::string_view key, T default_value) {
		Get(key, &default_value);
		return default_value;
	}

	template<>
	std::string_view Manager::Get(std::string_view key, std::string_view default_value) {
		auto iter = m_entries.find(key);
		if (iter != m_entries.end())
			return iter->second;
		Set(key, default_value);
		return default_value;
	}
}