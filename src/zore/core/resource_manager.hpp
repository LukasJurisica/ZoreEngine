#pragma once
#include <string_view>
#include <typeindex>
#include <type_traits>

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

		//typeid(T)

		template<typename T>
		T* Get(std::string_view name);

	private:

	};
}

#include "zore/core/resource_manager.inl"