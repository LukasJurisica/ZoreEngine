#include "core/Application.hpp"
#include "graphics/VertexLayout.hpp"
#include "utils/FileManager.hpp"
#include "utils/DataTypes.hpp"
#include "utils/Timer.hpp"
#include "voxel/ChunkManager.hpp"
#include "debug/Debug.hpp"

#include "math/MathUtils.hpp"

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
		//window.SetBorderless(true);
		window.HideCursor(true);
		//window.SetFullscreen(true);

		engine->SetVSync(false);
		engine->SetClearColour(0.5f, 0.8f, 0.92f);
		engine->SetClearMode({ BufferType::COLOUR, BufferType::DEPTH });
		engine->SetTopology(MeshTopology::TRIANGLE_STRIP);

		blockShader = Shader::Create("VoxelBlocks");
		spriteShader = Shader::Create("VoxelSprites");
		postProcessShader = Shader::Create("postprocess");

		postProcessShader->Bind();
		glm::ivec2 res = window.GetSize();
		postProcessShader->SetFloat2("resolution", { 1.f / res.x, 1.f / res.y });
		postProcessShader->SetTextureSlot("screen", 0);

		frameBuffer = FrameBuffer::Create(1920, 1080, 1, DepthFormat::DEPTH32);
		frameBuffer->GetTexture(0)->SetTextureSlot(0);
	}

	Application::~Application() {
		delete frameBuffer;
		delete postProcessShader;
		delete blockShader;
		delete spriteShader;
		delete engine;
	}

	void Application::Run() {
		// Create the Player and Camera uniform buffer
		Player player(&camera, { 32, 66, 32 });
		ShaderData shaderData = { camera.GetProjection(), camera.GetPosition(), 0.f };
		UniformBuffer* shaderDataBuffer = UniformBuffer::Create(&shaderData, sizeof(shaderData), BufferMode::DYNAMIC, 0);
		shaderDataBuffer->Bind();

		// Create uniform buffer with offsets for voxel vertex positions
		glm::ivec4 offsetData[] = {
			{ 0, 1, 0, 0}, { 0, 0, 0, 0}, { 0, 1, 1, 0}, { 0, 0, 1, 0}, // -x WEST
			{ 1, 1, 1, 0}, { 1, 0, 1, 0}, { 1, 1, 0, 0}, { 1, 0, 0, 0}, // +x EAST
			{ 0, 0, 1, 0}, { 0, 0, 0, 0}, { 1, 0, 1, 0}, { 1, 0, 0, 0}, // -y DOWN
			{ 0, 1, 0, 0}, { 0, 1, 1, 0}, { 1, 1, 0, 0}, { 1, 1, 1, 0}, // +y UP
			{ 1, 1, 0, 0}, { 1, 0, 0, 0}, { 0, 1, 0, 0}, { 0, 0, 0, 0}, // -z NORTH
			{ 0, 1, 1, 0}, { 0, 0, 1, 0}, { 1, 1, 1, 0}, { 1, 0, 1, 0}, // +z SOUTH
			{ 0, 1, 0, 0}, { 0, 0, 0, 0}, { 1, 1, 1, 0}, { 1, 0, 1, 0}, // Sprite A
			{ 0, 1, 1, 0}, { 0, 0, 1, 0}, { 1, 1, 0, 0}, { 1, 0, 0, 0}, // Sprite B
		};
		UniformBuffer* offsetDataBuffer = UniformBuffer::Create(&offsetData, sizeof(offsetData), BufferMode::STATIC, 1);
		offsetDataBuffer->Bind();

		// Create the mesh used for screenspace rendering and voxel faces
		VertexLayout* UBx1 = VertexLayout::Create("UBx1", blockShader, { {"vertexID", VertexDataType::UBYTE, 1} }, { {"face", VertexDataType::UINT, 2} }, 1u);
		ubyte vertices[] = { 0, 1, 2, 3 };
		Mesh* quad = Mesh::Create(&vertices, sizeof(vertices[0]), sizeof(vertices) / sizeof(vertices[0]));
		quad->Bind();

		// Initialize the chunk manager
		ChunkManager::Init(RENDER_DISTANCE, camera.GetPosition());

		Timer timer;
		float deltaTime, runningTime = 0;
		unsigned int frameCount = 0;

		while (!window.ShouldClose()) {
			// Update Time, framerate counter, and Camera --------
			deltaTime = timer.DeltaTime(true);
			runningTime += deltaTime;
			frameCount++;
			if (timer.TimeSinceStart() > 1) {
				Logger::Log("FrameRate: " + std::to_string(frameCount));
				frameCount = 0;
				timer.Reset();
			}
			player.Update(deltaTime);
			shaderData = { camera.GetProjection() * camera.GetView(), camera.GetPosition(), runningTime };
			shaderDataBuffer->Set(&shaderData, sizeof(shaderData));
			//shaderDataBuffer->Update(&shaderData, sizeof(shaderData));
			ChunkManager::Update(camera);


			// RENDER THE SCENE TO FRAMEBUFFER --------
			frameBuffer->Bind();
			//engine->SetWireframe(true);
			engine->SetBackFaceCulling(true);
			engine->SetDepthTest(true);
			engine->Clear();
			blockShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				blockShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(quad->GetCount(), chunk->GetChunkMesh(ChunkMeshType::BLOCKS).Bind());
			}

			engine->SetBackFaceCulling(false);
			spriteShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				spriteShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(quad->GetCount(), chunk->GetChunkMesh(ChunkMeshType::SPRITES).Bind());
			}

			//fluidShader->Bind();
			//for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
			//	fluidShader->SetInt3("chunkPos", chunk->GetPosition());
			//	engine->DrawLinearInstanced(quad->GetCount(), chunk->GetChunkMesh(ChunkMeshType::FLUIDS).Bind());
			//}

			// RENDER FRAMEBUFFER TEXTURE TO SCREEN --------
			frameBuffer->Unbind();
			//engine->SetWireframe(false);
			engine->SetDepthTest(false);
			postProcessShader->Bind();
			engine->DrawLinear(quad->GetCount());
			window.Update();
		}
		
		delete quad;
		delete UBx1;
		delete shaderDataBuffer;
		delete offsetDataBuffer;
	}

	void Application::OnWindowResize(int width, int height) {
		if (width && height) {
			engine->SetViewport(width, height);
			frameBuffer->SetSize(width, height);
			frameBuffer->GetTexture(0)->SetTextureSlot(0);
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