#include "zore/graphics/RenderEngine.hpp"
#include "zore/graphics/Shader.hpp"
#include "zore/devices/Window.hpp"
#include "zore/debug/Debug.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace zore {

	static uint32_t clearMode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	static uint32_t topology = GL_TRIANGLES;
	static uint32_t indexType = GL_UNSIGNED_INT;
	static uint64_t indexSize = sizeof(uint32_t);

	//========================================================================
	//	Render Engine Interface
	//========================================================================

	void RenderEngine::Init() {
		int version = gladLoadGL(glfwGetProcAddress);
		ENSURE(version, "Failed to initialize GLAD");
		version = (GLAD_VERSION_MAJOR(version) * 10) + GLAD_VERSION_MINOR(version);
		int context;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &context);
		Shader::SetShaderVersion("#version " + TOSTR(version * 10) + ((context & GL_CONTEXT_CORE_PROFILE_BIT) ? " core" : " compatibility"));

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		stbi_set_flip_vertically_on_load(true);
	}

	void RenderEngine::SetIndexType(IndexType type) {
		static const uint32_t IndexTypeToGLIndexType[] = { GL_UNSIGNED_SHORT, GL_UNSIGNED_INT };
		static const uint64_t IndexTypeToIndexSize[] = { sizeof(uint16_t), sizeof(uint32_t) };
		indexType = IndexTypeToGLIndexType[static_cast<uint32_t>(type)];
		indexSize = IndexTypeToIndexSize[static_cast<uint32_t>(type)];
	}

	void RenderEngine::ResetViewport() {
		const glm::ivec2& res = Window::GetSize();
		glViewport(0, 0, res.x, res.y);
	}

	void RenderEngine::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y) {
		glViewport(x, y, width, height);
	}

	void RenderEngine::SetBlending(bool value) {
		static bool blendingEnabled = false;
		if (blendingEnabled != value) {
			value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
			blendingEnabled = value;
		}
	}

	void RenderEngine::SetDepthWrite(bool value) {
		glDepthMask(value);
	}

	void RenderEngine::SetFaceCulling(bool value) {
		static bool faceCullingEnabled = false;
		if (faceCullingEnabled != value) {
			value ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
			faceCullingEnabled = value;
		}
	}

	void RenderEngine::SetFaceCulling(CullingMode mode) {
		static const uint32_t CullingModeToGLCullingMode[] = { GL_NONE, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK };
		static CullingMode faceCullingMode = CullingMode::BACK;

		SetFaceCulling(mode != CullingMode::NONE);
		if (mode != faceCullingMode) {
			glCullFace(CullingModeToGLCullingMode[static_cast<uint32_t>(mode)]);
			faceCullingMode = mode;
		}
	}

	void RenderEngine::SetDepthTest(bool value) {
		static bool depthTestEnabled = false;
		if (depthTestEnabled != value) {
			value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			depthTestEnabled = value;
		}
	}

	void RenderEngine::SetDepthTest(DepthTest mode) {
		static const uint32_t DepthModeToGLDepthMode[] = { GL_NEVER, GL_LESS, GL_LEQUAL, GL_EQUAL, GL_GEQUAL, GL_GREATER, GL_NOTEQUAL, GL_ALWAYS };
		static DepthTest depthTest = DepthTest::LESS;

		SetDepthTest(depthTest != DepthTest::ALWAYS);
		if (mode != depthTest) {
			glDepthFunc(DepthModeToGLDepthMode[static_cast<uint32_t>(mode)]);
			depthTest = mode;
		}
	}

	void RenderEngine::SetStencilTest(bool value) {
		static bool stencilTestEnabled = false;
		if (stencilTestEnabled != value) {
			value ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
			stencilTestEnabled = value;
		}

		MultidrawCommand temp{ 1 };
	}

	void RenderEngine::SetWireframe(bool value) {
		glPolygonMode(GL_FRONT_AND_BACK, value ? GL_LINE : GL_FILL);
	}

	void RenderEngine::SetVSync(bool value) {
		glfwSwapInterval(value);
	}

	void RenderEngine::SetClearColour(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void RenderEngine::SetClearMode(const std::vector<BufferType>& buffers) {
		static const uint32_t BufferTypeToGLBufferType[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT };
		clearMode = 0u;
		for (const BufferType& b : buffers)
			clearMode |= BufferTypeToGLBufferType[static_cast<int>(b)];
	}

	void RenderEngine::EnableColourChannels(bool r, bool g, bool b, bool a) {
		glColorMask(r, g, b, a);
	}

	void RenderEngine::SetTopology(MeshTopology t) {
		static const uint32_t MeshTopologyToGLMeshTopology[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };
		topology = MeshTopologyToGLMeshTopology[static_cast<int>(t)];
	}

	void RenderEngine::Clear() {
		glClear(clearMode);
	}

	void RenderEngine::DrawLinear(uint32_t count, uint32_t offset) {
		if (count > 0)
			glDrawArrays(topology, offset, count);
	}

	void RenderEngine::DrawIndexed(uint32_t count, uint32_t offset) {
		if (count > 0)
			glDrawElements(topology, count, indexType, reinterpret_cast<void*>(offset * indexSize));
	}

	void RenderEngine::DrawLinearInstanced(uint32_t vertexCount, uint32_t modelCount, uint32_t offset) {
		if (vertexCount > 0 && modelCount > 0)
			glDrawArraysInstanced(topology, offset, vertexCount, modelCount);
	}

	void RenderEngine::DrawIndexedInstanced(uint32_t indexCount, uint32_t modelCount, uint32_t offset) {
		if (indexCount > 0 && modelCount > 0)
			glDrawElementsInstanced(topology, indexCount, indexType, reinterpret_cast<void*>(offset * indexSize), modelCount);
	}

	//========================================================================
	//	Multidraw Command Buffer Class
	//========================================================================

	MultidrawCommand::MultidrawCommand(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t instanceOffset)
		: m_vertexCount(vertexCount), m_vertexOffset(vertexOffset), m_instanceCount(instanceCount), m_instanceOffset(instanceOffset) {}

	MultidrawCommandBuffer::MultidrawCommandBuffer(MultidrawCommand* data, uint32_t count, bool calculateInstanceOffsets) {
		glCreateBuffers(1, &m_id);
		Set(data, count, calculateInstanceOffsets);
	}

	MultidrawCommandBuffer::~MultidrawCommandBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	uint32_t MultidrawCommandBuffer::GetID() const {
		return m_id;
	}

	void MultidrawCommandBuffer::Set(MultidrawCommand* data, uint32_t count, bool calculateInstanceOffsets) {
		if (calculateInstanceOffsets)
			CalculateInstanceOffsets(data, count, 0);
		glNamedBufferData(m_id, sizeof(MultidrawCommand) * count, data, GL_STATIC_DRAW);
	}

	void MultidrawCommandBuffer::Update(MultidrawCommand* data, uint32_t count, uint32_t offset) {
		void* ptr = glMapNamedBufferRange(m_id, sizeof(MultidrawCommand) * offset, sizeof(MultidrawCommand) * count, GL_MAP_WRITE_BIT);
		memcpy(ptr, data, sizeof(MultidrawCommand) * count);
		glUnmapNamedBuffer(m_id);
	}

	void MultidrawCommandBuffer::Bind() const {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_id);
	}

	void MultidrawCommandBuffer::CalculateInstanceOffsets(MultidrawCommand* data, uint32_t count, uint32_t baseOffset) {
		uint32_t offset = baseOffset;
		for (uint32_t i = 0; i < count; i++) {
			data[i].m_instanceOffset = offset;
			offset += data[i].m_instanceCount;
		}
	}
}