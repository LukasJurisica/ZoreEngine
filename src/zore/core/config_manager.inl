#pragma once

#ifdef __INTELLISENSE__
#include "zore/core/config_manager.hpp"
#endif
#include "zore/utils/string.hpp"

namespace zore::Config {

	template<typename T>
	void Manager::Set(std::string_view key, T value) {
		m_entries[std::string(key)] = std::to_string(value);
	}

	template<typename T>
	void Manager::Get(std::string_view key, T* value) {
		auto iter = m_entries.find(key);
		if (iter != m_entries.end())
			String::Parse<T>(iter->second, value);
		else
			Set(key, *value);
	}

	template<typename T>
	T Manager::Get(std::string_view key, T default_value) {
		Get(key, &default_value);
		return default_value;
	}
}