#pragma once

#include "zore/utils/sized_integer.hpp"
#include <vector>
#include <string>

namespace zore {

	//========================================================================
	//	Render Engine Interface
	//========================================================================

	enum class BufferType { COLOUR, DEPTH, STENCIL };
	enum class IndexType { UINT16, UINT32 };
	enum class CullingMode { NONE, FRONT, BACK, BOTH };
	enum class DepthTest { NEVER, LESS, LESS_EQUAL, EQUAL, GREATER_EQUAL, GREATER, NOT_EQUAL, ALWAYS };
	enum class MeshTopology { POINT_LIST, LINE_LIST, LINE_STRIP, LINE_LOOP, TRIANGLE_LIST, TRIANGLE_STRIP, TRIANGLE_FAN };

	class RenderEngine {
	public:
		static void Init();
        static void Cleanup();

		static void ResetViewport();
		static void SetIndexType(IndexType type);
		static void SetViewport(uint32_t width, uint32_t height, uint32_t x = 0u, uint32_t y = 0u);
		static void SetBlending(bool value);
		static void SetDepthWrite(bool value);
		static void SetFaceCulling(bool value);
		static void SetFaceCulling(CullingMode mode);
		static void SetDepthTest(bool value);
		static void SetDepthTest(DepthTest mode);
		static void SetStencilTest(bool value);
		static void SetWireframe(bool value);
		static void SetVSync(bool value);
		static void SetClearDepthValue(float depth);
		static void SetClearStencilValue(int32_t stencil);
		static void SetClearColour(float r, float g, float b, float a = 1.0f);
		static void SetClearMode(const std::vector<BufferType>& buffers);
		static void SetTopology(MeshTopology topology);
		static void EnableColourChannels(bool r = true, bool g = true, bool b = true, bool a = true);
		static void Clear();
		static void DrawLinear(uint32_t count, uint32_t offset = 0u);
		static void DrawIndexed(uint32_t count, uint32_t offset = 0u);
		static void DrawLinearInstanced(uint32_t vertexCount, uint32_t modelCount, uint32_t offset = 0u);
		static void DrawIndexedInstanced(uint32_t indexCount, uint32_t modelCount, uint32_t offset = 0u);
	};

    /*
	//========================================================================
	//	Multidraw Command Buffer Class
	//========================================================================

	struct MultidrawCommand {
		MultidrawCommand(uint32_t vertex_count, uint32_t vertex_offset = 0, uint32_t instance_count = 1, uint32_t instance_offset = 0);
		uint32_t m_vertex_count;
		uint32_t m_vertex_offset;
		uint32_t m_instance_count;
		uint32_t m_instance_offset;
	};

	class MultidrawCommandBuffer {
	public:
		MultidrawCommandBuffer(MultidrawCommand* data, uint32_t count, bool calculate_instance_offsets = false);
		MultidrawCommandBuffer(const MultidrawCommandBuffer&) = delete;
		MultidrawCommandBuffer(MultidrawCommandBuffer&&) = delete;
		MultidrawCommandBuffer& operator=(const MultidrawCommandBuffer&) = delete;
		MultidrawCommandBuffer& operator=(MultidrawCommandBuffer&&) = delete;
		~MultidrawCommandBuffer();
		
		static void CalculateInstanceOffsets(MultidrawCommand* data, uint32_t count, uint32_t base_offset = 0u);

		uint32_t GetID() const;
		void Set(MultidrawCommand* data, uint32_t count, bool calculate_instance_offsets = false);
		void Update(MultidrawCommand* data, uint32_t count, uint32_t offset = 0u);
		void Bind() const;

	private:
		uint32_t m_id;
	};
    */
}