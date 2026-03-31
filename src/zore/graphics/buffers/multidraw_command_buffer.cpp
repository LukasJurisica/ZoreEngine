#include "zore/graphics/buffers/multidraw_command_buffer.hpp"
#include <glad/glad.h>

namespace zore::multidraw {

	LinearCommand::LinearCommand(uint32_t vertex_count, uint32_t vertex_offset, uint32_t instance_count, uint32_t instance_offset) :
		m_vertex_count(vertex_count), m_instance_count(instance_count), m_vertex_offset(vertex_offset), m_instance_offset(instance_offset) {}

	IndexedCommand::IndexedCommand(uint32_t index_count, uint32_t index_offset, uint32_t vertex_offset, uint32_t m_instance_count, uint32_t instance_offset) :
		m_index_count(index_count), m_instance_count(m_instance_count), m_index_offset(index_offset), m_vertex_offset(vertex_offset), m_instance_offset(instance_offset) {}

	//========================================================================
	//	Multidraw Command Buffer
	//========================================================================

	CommandBuffer::CommandBuffer(const void_span& span) : Base(span) {}

	CommandBuffer::CommandBuffer(const void* data, size_t size) : Base(data, size) {}

	void CommandBuffer::Bind() const {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, GetID());
	}
}