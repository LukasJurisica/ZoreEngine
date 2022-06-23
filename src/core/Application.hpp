#pragma once
#include "core/Camera.hpp"
#include "devices/Window.hpp"
#include "devices/Keyboard.hpp"
#include "graphics/RenderEngine.hpp"
#include "graphics/FrameBuffer.hpp"
#include "graphics/Sampler.hpp"
#include "graphics/Shader.hpp"

namespace zore {

	struct ShaderData {
		glm::mat4 vp_mat;
		glm::mat4 ivp_mat;
		glm::vec3 cameraPos;
		float time;
		glm::vec2 resolution;
	};

	struct textData {
		textData(ushort x, ushort y, ubyte width, ubyte scale, ushort offset) {
			a = (x << 16) + y;
			b = (width << 24) + (scale << 16) + offset;
		}
		uint a;
		uint b;
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

		Shader* blockShader;
		Shader* spriteShader;
		Shader* fluidShader;
		Shader* skyShader;
		Shader* debugLineShader;
		Shader* textShader;
		Shader* postProcessShader;
		
		Sampler* linearSampler;
		Sampler* nearestSampler;

		glm::vec2 inverseWindowResolution;
	};
}