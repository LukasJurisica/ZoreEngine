#include "core/Application.hpp"
#include "graphics/VertexLayout.hpp"
#include "utils/FileManager.hpp"
#include "utils/DataTypes.hpp"
#include "utils/Timer.hpp"
#include "voxel/ChunkManager.hpp"
#include "debug/Debug.hpp"

#include <iostream>

namespace zore {
	
	void Application::Init() {
		FileManager::Init();
		// load config from file
		RenderEngine::SetAPI(API::OPENGL);
		Window::Init();
		Application app;
		app.Run();
	}

	void Application::Cleanup() {
		ChunkManager::Cleanup();
		Window::Cleanup();
	}

#define RENDER_DISTANCE 16u

	Application::Application() : window(1920, 1080), engine(RenderEngine::Get()), camera(75.f, window.GetAspectRatio(), 0.1f, RENDER_DISTANCE * Chunk::CHUNK_WIDTH * 2) {
		frameBuffer = FrameBuffer::Create(1920, 1080, 1, DepthFormat::DEPTH32);
		postProcessShader = nullptr;

		engine->SetVSync(false);
		engine->SetBackFaceCulling(true);
		engine->SetClearColour(0.5f, 0.8f, 0.92f);
		engine->SetClearMode({ BufferType::COLOUR, BufferType::DEPTH });
		engine->SetTopology(MeshTopology::TRIANGLE_STRIP);

		//window.SetBorderless(true);
		window.HideCursor(true);
		//window.SetFullscreen(true);
	}

	void Application::Run() {
		// Create and compile shaders
		postProcessShader = Shader::Create("postprocess");
		postProcessShader->Bind();
		glm::ivec2 res = window.GetSize();
		postProcessShader->SetFloat2("resolution", { 1.f / res.x, 1.f / res.y });
		Shader* voxelShader = Shader::Create("voxelTerrain");

		// Create the camera controller and associated uniform buffer
		CameraController controller(&camera, 0.1f, {32, 66, 32});
		//ShaderData shaderData = { camera.GetProjection(), camera.GetPosition(), 0.f };
		//UniformBuffer* shaderDataBuffer = UniformBuffer::Create(&shaderData, sizeof(shaderData), BufferMode::DYNAMIC, 0);
		//shaderDataBuffer->Bind();

		// Create uniform buffer with offsets for voxel vertex positions
		glm::ivec4 offsetData[] = {
			{ 0, 1, 0, 0}, { 0, 0, 0, 0}, { 0, 1, 1, 0}, { 0, 0, 1, 0}, // -x WEST
			{ 1, 1, 1, 0}, { 1, 0, 1, 0}, { 1, 1, 0, 0}, { 1, 0, 0, 0}, // +x EAST
			{ 0, 0, 1, 0}, { 0, 0, 0, 0}, { 1, 0, 1, 0}, { 1, 0, 0, 0}, // -y DOWN
			{ 0, 1, 0, 0}, { 0, 1, 1, 0}, { 1, 1, 0, 0}, { 1, 1, 1, 0}, // +y UP
			{ 1, 1, 0, 0}, { 1, 0, 0, 0}, { 0, 1, 0, 0}, { 0, 0, 0, 0}, // -z NORTH
			{ 0, 1, 1, 0}, { 0, 0, 1, 0}, { 1, 1, 1, 0}, { 1, 0, 1, 0}, // +z SOUTH
		};
		UniformBuffer* offsetDataBuffer = UniformBuffer::Create(&offsetData, sizeof(offsetData), BufferMode::STATIC, 1);
		offsetDataBuffer->Bind();

		// Create the mesh used for screenspace rendering and voxel faces
		VertexLayout* UBx1 = VertexLayout::Create("UBx1", voxelShader, { {"vertexID", VertexDataType::UBYTE, 1} }, { {"face", VertexDataType::UINT, 2} }, 1u);
		ubyte vertices[] = { 0, 1, 2, 3 };
		Mesh* quad = Mesh::Create(&vertices, sizeof(vertices[0]), sizeof(vertices) / sizeof(vertices[0]));
		quad->Bind();

		// Initialize the chunk manager
		ChunkManager::Init(RENDER_DISTANCE, camera.GetPosition());

		Timer timer;
		float deltaTime, runningTime = 0;
		unsigned int frameCount = 0;

		while (!window.ShouldClose()) {
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
			//shaderData = { camera.GetProjection() * camera.GetView(), camera.GetPosition(), runningTime };
			//shaderDataBuffer->Update(&shaderData, sizeof(shaderData));

			// RENDER THE SCENE TO FRAMEBUFFER --------
			//engine->SetWireframe(true);
			engine->SetDepthTest(true);
			frameBuffer->Bind();
			voxelShader->Bind();
			engine->Clear();

			voxelShader->SetMat4("vp_mat", camera.GetProjection() * camera.GetView());
			voxelShader->SetFloat3("cameraPos", camera.GetViewPosition());

			ChunkManager::Update(camera.GetPosition());
			for (const std::pair<size_t, Chunk*>& pair : ChunkManager::GetChunks()) {
				Chunk* chunk = pair.second;
				if (chunk->ShouldBeDrawn(camera)) {
					voxelShader->SetInt3("chunkPos", chunk->GetPosition());
					engine->DrawLinearInstanced(quad->GetCount(), chunk->Bind());
				}
			}
			// END RENDER THE SCENE TO FRAMEBUFFER --------

			// RENDER FRAMEBUFFER TEXTURE TO SCREEN --------
			//engine->SetWireframe(false);
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
			postProcessShader->Bind();
			postProcessShader->SetFloat2("resolution", { 1.f / width, 1.f / height });
		}
		else {
			Logger::Log("Window Minimized");
		}
	}

	void Application::OnKeyPress(int key, int mods) {
		switch (key) {
		case KEY_ESCAPE:
			window.ToggleCursor();
			break;
		}
	}
}