#pragma once

#include "zore/graphics/buffers/buffer_base.hpp"

namespace zore::multidraw {

	//========================================================================
	//	Multidraw Command Structs
	//========================================================================

	struct LinearCommand {
		LinearCommand(uint32_t vertex_count, uint32_t vertex_offset = 0, uint32_t instance_count = 1, uint32_t instance_offset = 0);
		uint32_t m_vertex_count;
		uint32_t m_instance_count;
		uint32_t m_vertex_offset;
		uint32_t m_instance_offset;
	};

	struct IndexedCommand {
		IndexedCommand(uint32_t index_count, uint32_t index_offset = 0, uint32_t vertex_offset = 0, uint32_t m_instance_count = 1, uint32_t instance_offset = 0);
		uint32_t m_index_count;
		uint32_t m_instance_count;
		uint32_t m_index_offset;
		uint32_t m_vertex_offset;
		uint32_t m_instance_offset;
	};

	//========================================================================
	//	Multidraw Command Buffer
	//========================================================================

	class CommandBuffer : public Buffer::Base {
		CommandBuffer() = default;
		CommandBuffer(const void_span& span);
		CommandBuffer(const void* data, size_t size);
		CommandBuffer(CommandBuffer&&) = default;
		CommandBuffer& operator=(CommandBuffer&&) = default;
		~CommandBuffer() = default;

		void Bind() const;
	};
}