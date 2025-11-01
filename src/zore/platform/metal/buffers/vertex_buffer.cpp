#include "zore/graphics/buffers/vertex_buffer.hpp"
#include "zore/platform/metal/metal_core.hpp"

namespace zore {

    VertexBuffer::VertexBuffer(bool instanced) {

    }

    VertexBuffer::VertexBuffer(const VoidSpan& span, bool instanced) {
        Set(span);
    }

    VertexBuffer::VertexBuffer(const void* data, uint32_t size, uint32_t stride, bool instanced) {
        Set(data, size, stride);
    }

    VertexBuffer::~VertexBuffer() { 

    }

    uint32_t VertexBuffer::GetID() const {
        return m_id;
    }

    void VertexBuffer::Set(const VoidSpan& span) {

    }

    void VertexBuffer::Set(const void* data, uint32_t size, uint32_t stride) {

    }

    void VertexBuffer::Update(const void* data, uint32_t size, uint32_t offset) {

    }
    
    void VertexBuffer::Bind() const {

    }
}