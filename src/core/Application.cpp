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

		engine->SetBackFaceCulling(true);
		engine->SetVSync(false);
		engine->SetClearColour(0.2f, 0.3f, 0.3f);
		//engine->SetClearColour(0.0f, 0.0f, 0.0f, 0.0f);
		engine->SetClearMode({ BufferType::COLOUR });

		//window.SetBorderless(true);
		window.HideCursor(true);
	}

	struct Vertex {
		Byte x, y;
	};

	void Application::Run() {
		Shader* shader = Shader::Create("shader");
		shader->Bind();

		CameraController controller(&camera, 0.1f);
		ShaderData shaderData = { camera.GetProjection(), camera.GetPosition(), 0.f };
		UniformBuffer* shaderDataBuffer = UniformBuffer::Create(&shaderData, sizeof(ShaderData));
		shaderDataBuffer->Bind();

		// Create the screenspace quad
		VertexLayout* SBx2 = VertexLayout::Create("SBx2", shader, { { "position", VertexDataType::Byte, 2 } });
		Vertex vertices[] = {{-1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, -1}, {1, 1}};
		Mesh* mesh = Mesh::Create(&vertices, sizeof(vertices), sizeof(Vertex));
		mesh->Bind();

		Timer timer;
		float deltaTime = 0;
		float runningTime = 0;
		unsigned int frameCount = 0;

		while (!window.ShouldClose()) { 
			engine->Clear();

			// Update Time and framerate counter
			deltaTime = timer.DeltaTime(true);
			runningTime += deltaTime;
			frameCount++;
			if (timer.TimeSinceStart() > 1) {
				Logger::Log("FrameRate: " + std::to_string(frameCount));
				frameCount = 0;
				timer.Reset();
			}

			// Update uniform buffer with pertinant data
			controller.Update(deltaTime);
			shaderData = { camera.GetProjection() * camera.GetView(), camera.GetPosition(), runningTime };
			shaderDataBuffer->Update(&shaderData, sizeof(shaderData));

			engine->Draw(mesh);
			window.Update();
		}
	}

	void Application::OnWindowResize(int width, int height) {
		if (width && height) {
			engine->SetViewport(width, height);
			camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
		}
	}

	void Application::OnKeyPress(unsigned char key, int mods) {

	}
}