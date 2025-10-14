#pragma once

#include "zore/utils/sized_integer.hpp"
#include <vector>

namespace zore {

	enum class Barrier {
		ALL,
		VERTEX_ATTRIB_ARRAY,
		ELEMENT_ARRAY,
		UNIFORM_BUFFER,
		SHADER_STORAGE_BUFFER,
		CLIENT_MAPPED_BUFFER,
		QUERY_BUFFER,
		FRAMEBUFFER,
		PIXEL_BUFFER,
		BUFFER_UPDATE,
		TEXTURE_FETCH,
		TEXTURE_UPDATE,
		SHADER_IMAGE_ACCESS,
		TRANSFORM_FEEDBACK,
		ATOMIC_COUNTER,
		COMMAND
	};

	class MemoryBarrier {
	public:
		MemoryBarrier(Barrier barrier);
		MemoryBarrier(const std::vector<Barrier>& barriers = {});
		~MemoryBarrier() = default;

		void Apply() const;
		void Add(Barrier barrier);
		void Add(const std::vector<Barrier>& barriers);
		void Remove(Barrier barrier);
		void Remove(const std::vector<Barrier>& barriers);
		void Clear();

	private:
		uint32_t m_barrier;
	};
}