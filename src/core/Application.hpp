#pragma once
#include "core/Camera.hpp"
#include "devices/Window.hpp"
#include "devices/Keyboard.hpp"
#include "graphics/RenderEngine.hpp"
#include "graphics/FrameBuffer.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Shader.hpp"

namespace zore {

	struct DynamicShaderData {
		glm::mat4 vp_mat;
		glm::mat4 inv_vp_mat;
		glm::vec3 cam_pos;
		float time;
	};

	struct StaticShaderData {
		glm::vec2 inv_res;
		float near_dist;
		float far_dist;
	};

	class Application : WindowListener, KeyListener {
	public:
		Application();
		~Application();
		static void Init();
		static void Cleanup();
		void Run();

	protected:
		void OnWindowResize(int width, int height) override;
		void OnKeyPress(int key, int mods) override;

	private:
		Camera camera;
		RenderEngine* engine;
		FrameBuffer* frameBuffer;

		DynamicShaderData dynamicShaderData;
		StaticShaderData staticShaderData;
		UniformBuffer* dynamicShaderDataBuffer;
		UniformBuffer* staticShaderDataBuffer;
		UniformBuffer* offsetDataBuffer;

		Shader* blockShader;
		Shader* spriteShader;
		Shader* fluidShader;
		Shader* skyShader;
		Shader* debugLineShader;
		Shader* textShader;
		Shader* postProcessShader;
		
		Texture::Sampler* linearSampler;
		Texture::Sampler* nearestSampler;
	};
}