#pragma once
#include "zore/structures/string_unordered_map.hpp"
#include <typeindex>

namespace zore::Resource {

	//========================================================================
	//	Resource Manager
	//========================================================================

	class Manager {
	public:
		void Cleanup();

		template<typename T, typename... Args>
			requires std::constructible_from<T, Args...>
		T* Create(std::string_view name, Args... args);

		template<typename T>
		bool Exists(std::string_view name);

		template<typename T>
		T* Get(std::string_view name);

		template<typename T>
		bool Destroy(std::string_view name);

	private:
		static inline std::unordered_map<std::type_index, zore::string_unordered_map<void*>> s_resources;
	};
}

#include "zore/core/resource_manager.inl"