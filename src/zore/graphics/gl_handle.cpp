#pragma once

#include "zore/graphics/gl_handle.hpp"
#include <glad/glad.h>

namespace zore {

	static std::vector<GLHandleRegistry*> s_registries;

	GLHandleRegistry::GLHandleRegistry(void (*deleter)(uint32_t&)) : m_deleter(deleter) {
		s_registries.push_back(this);
	}

	void GLHandleRegistry::Cleanup() {
		for (GLHandleRegistry* registry : s_registries)
			for (uint32_t& id : registry->m_pool)
				registry->m_deleter(id);
	}
}