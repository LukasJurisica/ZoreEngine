#pragma once

#include "zore/structures/string_unordered_map.hpp"
#include <stdexcept>
#include <any>

namespace zore::Command {

	//========================================================================
	//	Command Context
	//========================================================================
	
	class Context {
	public:
		Context(zore::string_unordered_map<std::any> values) : m_values(std::move(values)) {}

		template <typename T>
		const T& Get(std::string_view key) const {
			auto iter = m_values.find(key);
			if (iter == m_values.end())
				throw std::logic_error("Missing context argument: " + std::string(key));
			return std::any_cast<const T&>(iter->second);
		}

		bool Has(std::string_view key) const {
			return m_values.contains(key);
		}

	private:
		zore::string_unordered_map<std::any> m_values;
	};
}