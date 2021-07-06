#include "core/Application.hpp"
#include "debug/Console.hpp"
#include "debug/Debug.hpp"

#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Mesh.hpp"

namespace zore {

	void Application::EntryPoint() {
        DEBUG_ONLY(Console console);
		// Set the Rendering API
		RenderEngine::SetApi(API::OpenGL);
		Window::Init();
		{
			Application app;
			app.Run();
		}
		Window::Cleanup();
	}

	Application::Application() : window("main", 1920, 1080), engine(RenderEngine::Create()) {

	}

	void Application::Run() {
		Shader* s = Shader::Create("shader");
		s->Bind();
		BufferLayout::Create("basic", s, { { "position", ShaderDataType::Byte, 2, false } });

		// Create the screenspace quad - Used for both raymarching and postprocessing steps
		char vertices[] = { -1, 1, -1, -1, 1, -1, -1, 1, 1, -1, 1, 1 };
		Mesh* m = Mesh::Create(&vertices, sizeof(vertices), "basic");
		m->Bind();

		engine->SetClearColour(0.2f, 0.3f, 0.3f);

		while (!window.ShouldClose()) {
			window.Update();

			engine->Clear();
			engine->Draw(m);
		}
	}

	void Application::OnWindowResize(int width, int height) {
		engine->SetViewport(0, 0, width, height);
	}
}