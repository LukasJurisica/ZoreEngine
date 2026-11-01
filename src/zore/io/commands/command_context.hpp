#pragma once

#include "zore/io/commands/argument_parser.hpp"
#include "zore/structures/string_unordered_map.hpp"

namespace zore::Command {

	//========================================================================
	//	Command Context
	//========================================================================
	
	class Context {
	public:
		Context() = default;

		void Set(std::string_view key, std::string_view value) {
			m_values.emplace(std::string(key), value);
		}

		template <Parsable T>
		T Get(std::string_view key) const {
			auto iter = m_values.find(key);
			if (iter == m_values.end())
				throw ParseException("Missing context argument: " + std::string(key));
			return ArgumentParser<T>::Parse(iter->second);
		}

		bool Has(std::string_view key) const {
			return m_values.contains(key);
		}

		std::string& Response() {
			return m_response;
		}

	private:
		zore::string_unordered_map<std::string_view> m_values;
		std::string m_response;
	};
}