#pragma once

#include "zore/graphics/gl_handle.hpp"
#include <algorithm>

namespace zore {

	static std::vector<GLHandleRegistry*> s_registries;

	GLHandleRegistry::GLHandleRegistry(void (*deleter)(uint32_t&)) : m_deleter(deleter) {
		s_registries.push_back(this);
	}

	GLHandleRegistry::~GLHandleRegistry() {
		auto iter = std::find(s_registries.begin(), s_registries.end(), this);
		if (iter != s_registries.end()) {
			*iter = s_registries.back();
			s_registries.pop_back();
		}
	}

	void GLHandleRegistry::Cleanup() {
		for (GLHandleRegistry* registry : s_registries)
			for (uint32_t& id : registry->m_pool)
				registry->m_deleter(id);
	}
}