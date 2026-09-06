#pragma once

#include "zore/structures/object_pool.hpp"

namespace zore {

	class GLHandleRegistry {
	public:
		GLHandleRegistry(void (*deleter)(uint32_t&));
		~GLHandleRegistry() = default;

		object_pool<uint32_t, uint32_t>* operator->() { return &m_pool; }
		const object_pool<uint32_t, uint32_t>* operator->() const { return &m_pool; }
		uint32_t& operator[](uint32_t index) { return m_pool[index]; }
		const uint32_t& operator[](uint32_t index) const { return m_pool[index]; }

		static void Cleanup();

	public:
		object_pool<uint32_t, uint32_t> m_pool;
		void (*m_deleter)(uint32_t&);
	};
}