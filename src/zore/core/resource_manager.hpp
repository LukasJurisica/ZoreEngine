#pragma once
#include "zore/structures/string_unordered_map.hpp"
#include <typeindex>

namespace zore::Resource {

	//========================================================================
	//	Resource Manager
	//========================================================================

	class Manager {
	public:
		static void Cleanup();

		template<typename T, typename... Args>
			requires std::constructible_from<T, Args...>
		static T* Create(std::string_view name, Args... args);

		template<typename T>
		static bool Exists(std::string_view name);

		template<typename T>
		static T* Get(std::string_view name);

		template<typename T>
		static bool Destroy(std::string_view name);

	private:
		static inline std::unordered_map<std::type_index, zore::string_unordered_map<void*>> s_resources;
	};
}

#include "zore/core/resource_manager.inl"