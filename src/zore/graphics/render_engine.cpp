#include "zore/graphics/render_engine.hpp"
#include "zore/graphics/shader.hpp"
#include "zore/graphics/buffers/multidraw_command_buffer.hpp"
#include "zore/devices/window.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zore {

	static uint32_t s_clear_mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	static uint32_t s_topology = GL_TRIANGLES;
	static uint32_t s_index_type = GL_UNSIGNED_INT;
	static uint64_t s_index_size = sizeof(uint32_t);
	static std::string s_shader_version;

	//========================================================================
	//	Render Engine Utility
	//========================================================================

	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param) {
		if (type == GL_DEBUG_TYPE_ERROR)
			Logger::Error("GL ERROR: (", severity, ")", message);
		else if (id != 131185) // Using Video Memory
			Logger::Info("GL INFO:", message);
	}

	void RenderEngine::SetGLFeature(uint32_t feature, bool& current, bool value) {
		if (current != value) {
			value ? glEnable(feature) : glDisable(feature);
			current = value;
		}
	}

	void* RenderEngine::GetIndexOffset(uint32_t index_offset) {
		return reinterpret_cast<void*>(index_offset * s_index_size);
	}

	void* RenderEngine::GetCommandOffset(uint32_t command_offset, uint32_t stride) {
		return reinterpret_cast<void*>(command_offset * stride);
	}

	//========================================================================
	//	Render Engine Interface
	//========================================================================

	void RenderEngine::Init() {
		int version = gladLoadGL(glfwGetProcAddress);
		ENSURE(version, "Failed to initialize GLAD");
		int context;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &context);
		Shader::SetShaderVersion(GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version), context & GL_CONTEXT_CORE_PROFILE_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (IS_DEBUG) {
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(MessageCallback, 0);
		}
	}

	void RenderEngine::ResetViewport() {
		const zm::ivec2& res = Window::GetSize();
		glViewport(0, 0, res.x, res.y);
	}

	void RenderEngine::SetIndexType(IndexType type) {
		static const uint32_t s_index_type_to_gl_index_type[] = { GL_UNSIGNED_SHORT, GL_UNSIGNED_INT };
		static const uint64_t s_index_type_to_index_size[] = { sizeof(uint16_t), sizeof(uint32_t) };
		s_index_type = s_index_type_to_gl_index_type[static_cast<uint32_t>(type)];
		s_index_size = s_index_type_to_index_size[static_cast<uint32_t>(type)];
	}

	void RenderEngine::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y) {
		glViewport(x, y, width, height);
	}

	void RenderEngine::SetBlending(bool value) {
		static bool s_blending_enabled = false;
		SetGLFeature(GL_BLEND, s_blending_enabled, value);
	}

	void RenderEngine::SetDepthWrite(bool value) {
		glDepthMask(value);
	}

	void RenderEngine::SetFaceCulling(bool value) {
		static bool s_face_culling_enabled = false;
		SetGLFeature(GL_CULL_FACE, s_face_culling_enabled, value);
	}

	void RenderEngine::SetFaceCulling(CullingMode mode) {
		static const uint32_t s_culling_mode_to_gl_culling_mode[] = { GL_NONE, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK };
		static CullingMode s_face_culling_mode = CullingMode::BACK;

		SetFaceCulling(mode != CullingMode::NONE);
		if (mode != CullingMode::NONE && mode != s_face_culling_mode)
			glCullFace(s_culling_mode_to_gl_culling_mode[static_cast<uint32_t>(mode)]);
		s_face_culling_mode = mode;
	}

	void RenderEngine::SetDepthTest(bool value) {
		static bool s_depth_test_enabled = false;
		SetGLFeature(GL_DEPTH_TEST, s_depth_test_enabled, value);
	}

	void RenderEngine::SetDepthTest(DepthTest mode) {
		static const uint32_t s_depth_mode_to_gl_depth_mode[] = { GL_NEVER, GL_LESS, GL_LEQUAL, GL_EQUAL, GL_GEQUAL, GL_GREATER, GL_NOTEQUAL, GL_ALWAYS };
		static DepthTest s_depth_test = DepthTest::LESS;

		SetDepthTest(s_depth_test != DepthTest::ALWAYS);
		if (mode != s_depth_test) {
			glDepthFunc(s_depth_mode_to_gl_depth_mode[static_cast<uint32_t>(mode)]);
			s_depth_test = mode;
		}
	}

	void RenderEngine::SetStencilTest(bool value) {
		static bool s_stencil_test_enabled = false;
		SetGLFeature(GL_STENCIL_TEST, s_stencil_test_enabled, value);
	}

	void RenderEngine::SetWireframe(bool value) {
		glPolygonMode(GL_FRONT_AND_BACK, value ? GL_LINE : GL_FILL);
	}

	void RenderEngine::SetVSync(bool value) {
		glfwSwapInterval(value);
	}

	void RenderEngine::SetClearDepthValue(float depth) {
		glClearDepth(depth);
	}

	void RenderEngine::SetClearStencilValue(int32_t stencil) {
		glClearStencil(stencil);
	}

	void RenderEngine::SetClearColour(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void RenderEngine::SetClearMode(const std::vector<BufferType>& buffers) {
		static const uint32_t s_buffer_type_to_gl_buffer_type[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT };
		s_clear_mode = 0u;
		for (const BufferType& b : buffers)
			s_clear_mode |= s_buffer_type_to_gl_buffer_type[static_cast<int>(b)];
	}

	void RenderEngine::SetTopology(MeshTopology t) {
		static const uint32_t s_mesh_topology_to_gl_mesh_topology[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };
		s_topology = s_mesh_topology_to_gl_mesh_topology[static_cast<int>(t)];
	}

	void RenderEngine::EnableColourChannels(bool r, bool g, bool b, bool a) {
		glColorMask(r, g, b, a);
	}

	void RenderEngine::Clear() {
		glClear(s_clear_mode);
	}

	void RenderEngine::DrawLinear(uint32_t vertex_count, uint32_t offset) {
		if (vertex_count > 0)
			glDrawArrays(s_topology, offset, vertex_count);
	}

	void RenderEngine::DrawIndexed(uint32_t index_count, uint32_t offset) {
		if (index_count > 0)
			glDrawElements(s_topology, index_count, s_index_type, GetIndexOffset(offset));
	}

	void RenderEngine::DrawLinearInstanced(uint32_t vertex_count, uint32_t object_count, uint32_t offset) {
		if (vertex_count > 0 && object_count > 0)
			glDrawArraysInstanced(s_topology, offset, vertex_count, object_count);
	}

	void RenderEngine::DrawIndexedInstanced(uint32_t index_count, uint32_t object_count, uint32_t offset) {
		if (index_count > 0 && object_count > 0)
			glDrawElementsInstanced(s_topology, index_count, s_index_type, GetIndexOffset(offset), object_count);
	}

	void RenderEngine::MultiDrawLinearIndirect(uint32_t command_count, uint32_t command_offset, uint32_t stride) {
		stride = stride ? stride : static_cast<uint32_t>(sizeof(multidraw::LinearCommand));
		if (command_count > 0)
			glMultiDrawArraysIndirect(s_topology, GetCommandOffset(command_offset, stride), command_count, stride);
	}

	void RenderEngine::MultiDrawIndexedIndirect(uint32_t command_count, uint32_t command_offset, uint32_t stride) {
		stride = stride ? stride : static_cast<uint32_t>(sizeof(multidraw::IndexedCommand));
		if (command_count > 0)
			glMultiDrawElementsIndirect(s_topology, s_index_type, GetCommandOffset(command_offset, stride), command_count, stride);
	}
}