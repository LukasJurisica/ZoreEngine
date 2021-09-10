#include "core/Application.hpp"
#include "graphics/Shader.hpp"
#include "utils/FileManager.hpp"
#include "utils/DataTypes.hpp"
#include "utils/Timer.hpp"
#include "debug/Debug.hpp"

#include "math/MathUtils.hpp"
#include "math/WhiteNoise.hpp"

namespace zore {
	
	void Application::Init() {
		FileManager::Init();
		// load config from file
		RenderEngine::SetAPI(zore::API::OpenGL);
		Window::Init();
		Application app;
		app.Run();
	}

	void Application::Cleanup() {
		zore::Window::Cleanup();
	}

	Application::Application() :
		window(1920, 1080),
		engine(RenderEngine::Get()),
		camera(90.f, window.GetAspectRatio(), 0.1f, 1000.f) {

		frameBuffer = FrameBuffer::Create(1920, 1080, 1, DepthFormat::DEPTH32_TEXTURE);

		engine->SetBackFaceCulling(true);
		engine->SetVSync(false);
		engine->SetClearColour(0.2f, 0.3f, 0.3f);
		//engine->SetClearColour(0.0f, 0.0f, 0.0f, 0.0f);
		engine->SetClearMode({ BufferType::COLOUR, BufferType::DEPTH });

		//window.SetBorderless(true);
		//window.HideCursor(true);
	}

	struct Vertex {
		byte x, y;
	};

	void Application::Run() {
		Shader* postProcessShader = Shader::Create("postprocess");
		Shader* defaultShader = Shader::Create("shader");
		defaultShader->Bind();

		CameraController controller(&camera, 0.1f);
		ShaderData shaderData = { camera.GetProjection(), camera.GetPosition(), 0.f };
		UniformBuffer* shaderDataBuffer = UniformBuffer::Create(&shaderData, sizeof(ShaderData));
		shaderDataBuffer->Bind();

		// Create the screenspace quad
		VertexLayout* SBx2 = VertexLayout::Create("SBx2", defaultShader, { { "position", VertexDataType::BYTE, 2 } });
		Vertex vertices[] = {{-1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, -1}, {1, 1}};
		Mesh* quad = Mesh::Create(&vertices, sizeof(vertices), sizeof(Vertex));
		quad->Bind();

		Timer timer;
		float deltaTime = 0;
		float runningTime = 0;
		unsigned int frameCount = 0;

		while (!window.ShouldClose()) {
			// Update Time and framerate counter
			deltaTime = timer.DeltaTime(true);
			runningTime += deltaTime;
			frameCount++;
			if (timer.TimeSinceStart() > 1) {
				//Logger::Log("FrameRate: " + std::to_string(frameCount));
				frameCount = 0;
				timer.Reset();
			}

			// Update uniform buffer with pertinant data
			controller.Update(deltaTime);
			shaderData = { camera.GetProjection() * camera.GetView(), camera.GetPosition(), runningTime };
			shaderDataBuffer->Update(&shaderData, sizeof(shaderData));

			// RENDER THE SCENE TO FRAMEBUFFER --------
			engine->SetDepthTest(true);
			frameBuffer->Bind();
			engine->Clear();
			defaultShader->Bind();

			defaultShader->SetFloat4("model", { 1.0f, 0.0f,-1.0f, 0.5f });
			defaultShader->SetFloat4("color", { 1.0f, 0.5f, 0.2f, 1.0f });
			engine->DrawLinear(quad->GetCount());

			defaultShader->SetFloat4("model", {-1.0f, 0.0f,-1.0f, 0.5f });
			defaultShader->SetFloat4("color", { 0.2f, 0.5f, 1.0f, 1.0f });
			engine->DrawLinear(quad->GetCount());

			defaultShader->SetFloat4("model", { 0.0f, 0.2f,-1.5f, 1.0f });
			defaultShader->SetFloat4("color", { 0.5f, 1.0f, 0.2f, 1.0f });
			engine->DrawLinear(quad->GetCount());
			// END RENDER THE SCENE TO FRAMEBUFFER --------

			// RENDER FRAMEBUFFER TEXTURE TO SCREEN --------
			engine->SetDepthTest(false);
			frameBuffer->Unbind();
			postProcessShader->Bind();
			engine->DrawLinear(quad->GetCount());
			// END RENDER FRAMEBUFFER TEXTURE TO SCREEN --------

			window.Update();
		}
	}

	void Application::OnWindowResize(int width, int height) {
		if (width && height) {
			engine->SetViewport(width, height);
			frameBuffer->SetSize(width, height);
			camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
		}
		else {
			Logger::Log("Window Minimized");
		}
	}

	void Application::OnKeyPress(unsigned char key, int mods) {

	}
}