#include "zore/graphics/memory_barrier.hpp"
#include <glad/glad.h>

namespace zore {
	
	static const uint32_t s_barrier_to_gl_barrier_bit[] = { 
		GL_ALL_BARRIER_BITS,
		GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
		GL_ELEMENT_ARRAY_BARRIER_BIT,
		GL_UNIFORM_BARRIER_BIT,
		GL_SHADER_STORAGE_BARRIER_BIT,
		GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT,
		GL_QUERY_BUFFER_BARRIER_BIT,
		GL_FRAMEBUFFER_BARRIER_BIT,
		GL_PIXEL_BUFFER_BARRIER_BIT,
		GL_BUFFER_UPDATE_BARRIER_BIT,
		GL_TEXTURE_FETCH_BARRIER_BIT,
		GL_TEXTURE_UPDATE_BARRIER_BIT,
		GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
		GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
		GL_ATOMIC_COUNTER_BARRIER_BIT,
		GL_COMMAND_BARRIER_BIT
	};

	//========================================================================
	//	Memory Barrier
	//========================================================================

	MemoryBarrier::MemoryBarrier(Barrier barrier) : m_barrier(0) {
		Add(barrier);
		Apply();
	}

	MemoryBarrier::MemoryBarrier(const std::vector<Barrier>& barriers) : m_barrier(0) {
		Add(barriers);
		Apply();
	}

	void MemoryBarrier::Apply() const {
		glMemoryBarrier(m_barrier);
	}

	void MemoryBarrier::Add(Barrier barrier) {
		m_barrier |= s_barrier_to_gl_barrier_bit[static_cast<uint32_t>(barrier)];
	}

	void MemoryBarrier::Add(const std::vector<Barrier>& barriers) {
		for (const Barrier& barrier : barriers)
			Add(barrier);
	}

	void MemoryBarrier::Remove(Barrier barrier) {
		m_barrier &= ~s_barrier_to_gl_barrier_bit[static_cast<uint32_t>(barrier)];
	}

	void MemoryBarrier::Remove(const std::vector<Barrier>& barriers) {
		for (const Barrier& barrier : barriers)
			Remove(barrier);
	}

	void MemoryBarrier::Clear() {
		m_barrier = 0;
	}
}