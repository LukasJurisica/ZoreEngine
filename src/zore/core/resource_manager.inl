#pragma once

#ifdef __INTELLISENSE__
#include "zore/core/resource_manager.hpp"
#endif

namespace zore::Resource {

	template<typename T, typename... Args>
		requires std::constructible_from<T, Args...>
	T* Manager::Create(std::string_view name, Args... args) {
		T* resource = new T(args...);
		std::type_info const& type = typeid(T);
		if (s_resources.find(type) == s_resources.end())
			s_resources.emplace(type, zore::string_unordered_map<void*>());
		s_resources[type].emplace(name, resource);
		return resource;
	}

	template<typename T>
	bool Manager::Exists(std::string_view name) {
		auto iter = s_resources.find(std::type_index(typeid(T)));
		return iter != s_resources.end() && iter->second.find(name) != iter->second.end();
	}

	template<typename T>
	T* Manager::Get(std::string_view name) {
		auto iter = s_resources.find(std::type_index(typeid(T)));
		if (iter != s_resources.end()) {
			auto res = iter->second.find(name);
			return res != iter->second.end() ? static_cast<T*>(res->second) : nullptr;
		}
		return nullptr;
	}

	template<typename T>
	bool Manager::Destroy(std::string_view name) {
		auto iter = s_resources.find(std::type_index(typeid(T)));
		if (iter != s_resources.end()) {
			auto& resources = iter->second;
			auto resource_iter = resources.find(name);
			if (resource_iter != resources.end()) {
				delete resource_iter->second;
				resources.erase(resource_iter);
				return true;
			}
		}
		return false;
	}
}