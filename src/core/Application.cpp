#include "core/Application.hpp"
#include "graphics/VertexLayout.hpp"
#include "utils/FileManager.hpp"
#include "utils/Timer.hpp"
#include "debug/Debug.hpp"

#include "voxel/ChunkManager.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

#include <iostream>

#define WINDOW_SIZE 1920, 1080
#define RENDER_DISTANCE 16u

namespace zore {
	
	void Application::Init() {
		FileManager::Init();
		// load config from file
		RenderEngine::SetAPI(API::OPENGL);
		Window::Init(WINDOW_SIZE);
		RenderEngine::Init();

		Application app;
		app.Run();
	}

	void Application::Cleanup() {
		ChunkManager::Cleanup();
		Window::Cleanup();
	}

	Application::Application() : engine(RenderEngine::Get()), camera(75.f, Window::GetAspectRatio(), 0.1f, 1200.f) {
		//Window::SetBorderless(true);
		Window::HideCursor(true);
		//Window::SetFullscreen(true);
		glm::ivec2 res = Window::GetSize();
		inverseWindowResolution = { 1.f / res.x, 1.f / res.y };

		engine->SetVSync(false);
		//engine->SetClearColour(0.5f, 0.8f, 0.92f);
		engine->SetClearMode({ BufferType::DEPTH });
		engine->SetTopology(MeshTopology::TRIANGLE_STRIP);

		// Compile Shaders
		blockShader = Shader::Create("VoxelBlocks");
		fluidShader = Shader::Create("VoxelFluids");
		spriteShader = Shader::Create("VoxelSprites");
		skyShader = Shader::Create("Sky");
		debugLineShader = Shader::Create("DebugLines");
		textShader = Shader::Create("Text");
		postProcessShader = Shader::Create("PostProcess");
		
		// Set texture slots for shaders
		postProcessShader->Bind();
		postProcessShader->SetTextureSlot("screen", 0);
		spriteShader->Bind();
		spriteShader->SetTextureSlot("sprites", 1);

		// Create framebuffer
		frameBuffer = FrameBuffer::Create(WINDOW_SIZE, 1, TextureFormat::RGBA, DepthFormat::DEPTH24_STENCIL8);
		frameBuffer->GetTextureArray()->Bind(0);

		// Create Texture samplers and bind them to their respective texture units
		linearSampler = Sampler::Create(SampleMode::LINEAR);
		linearSampler->Bind(0);
		linearSampler->Bind(2);
		nearestSampler = Sampler::Create(SampleMode::NEAREST);
		nearestSampler->Bind(1);
	}

	Application::~Application() {
		delete linearSampler;
		delete nearestSampler;
		delete frameBuffer;

		delete blockShader;
		delete fluidShader;
		delete spriteShader;
		delete skyShader;
		delete debugLineShader;
		delete postProcessShader;

		delete engine;
	}

	void Application::Run() {
		// Create the Player and Camera uniform buffer
		Player player(&camera, { 32, 128, 32 });
		ShaderData shaderData = {};
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
		VertexLayout* UBx1 = VertexLayout::Create(blockShader, { {"vertexID", VertexDataType::UBYTE, 1} }, { {"face", VertexDataType::UINT, 2} }, 1u);
		ubyte vertices[] = { 0, 1, 2, 3 };
		Mesh* quadMesh = Mesh::Create(&vertices, sizeof(vertices[0]), sizeof(vertices) / sizeof(vertices[0]));
		// Create texture array for sprites;
		Texture2DArray* spriteTexture = Texture2DArray::Create({ "grass.png", "mush1.png" }, "assets/textures/", TextureFormat::RGBA);
		spriteTexture->Bind(1);

		// Initialize the chunk manager
		ChunkManager::Init(RENDER_DISTANCE, camera.GetPosition());

		//Bind mesh and vertex layouts for rendering
		engine->SetTopology(MeshTopology::TRIANGLE_STRIP);
		UBx1->Bind();
		quadMesh->Bind();

		// Create Text stuff (Temp)
		textData temp = textData(100, 100, 10, 2, 0);
		std::vector<textData> textInstanceData = { temp };
		InstanceArrayBuffer* tiab = InstanceArrayBuffer::Create(textInstanceData.data(), sizeof(textData) * textInstanceData.size(), sizeof(textData));
		// Create texture array for text;
		std::vector<std::string> textFilenames = { "65.png", "66.png" };
		Texture2DArray* textTexture = Texture2DArray::Create(textFilenames, "assets/fonts/consola/", TextureFormat::RGBA);
		textTexture->Bind(2);

		Timer timer;
		float deltaTime, runningTime = 0;
		unsigned int frameCount = 0;

		while (!Window::ShouldClose()) {
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

			glm::mat4 inv_vp_mat = glm::transpose(camera.GetView()) * glm::inverse(camera.GetProjection());
			shaderData = { camera.GetProjection() * camera.GetView(), inv_vp_mat, camera.GetPosition(), runningTime, inverseWindowResolution };
			shaderDataBuffer->Set(&shaderData, sizeof(shaderData));
			//shaderDataBuffer->Update(&shaderData, sizeof(shaderData));

			// RENDER THE SCENE TO FRAMEBUFFER --------
			frameBuffer->Bind();

			//engine->SetWireframe(true);
			engine->SetDepthTest(true);
			engine->Clear();

			// Render terrain and generic blocks
			blockShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				blockShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(quadMesh->GetCount(), chunk->GetChunkMesh(ChunkMeshType::BLOCKS).Bind());
			}
			// Render sprites (plants)
			engine->SetBackFaceCulling(false);
			spriteShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				spriteShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(quadMesh->GetCount(), chunk->GetChunkMesh(ChunkMeshType::SPRITES).Bind());
			}
			// Render fluids
			fluidShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				fluidShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(quadMesh->GetCount(), chunk->GetChunkMesh(ChunkMeshType::FLUIDS).Bind());
			}

			// Render Sky
			skyShader->Bind();
			engine->DrawLinear(quadMesh->GetCount());

			// Render debug for block that is currently selected
			glm::ivec3 pos;
			if (World::RaycastBlock(camera.GetPosition(), camera.GetForward(), pos, 10)) {
				debugLineShader->Bind();
				debugLineShader->SetInt3("pos", pos);
				engine->DrawLinearInstanced(quadMesh->GetCount(), 6);
			}
			engine->SetBackFaceCulling(true);
			engine->SetDepthTest(false);

			// RENDER FRAMEBUFFER TEXTURE TO SCREEN --------
			frameBuffer->Unbind();
			//engine->SetWireframe(false);
			postProcessShader->Bind();
			engine->DrawLinear(quadMesh->GetCount());

			// RENDER User Interface --------
			textShader->Bind();
			tiab->Bind();
			engine->DrawLinear(quadMesh->GetCount());

			Window::Update();
		}
		
		delete quadMesh;
		delete UBx1;
		delete shaderDataBuffer;
		delete offsetDataBuffer;

		delete tiab;
		delete textTexture;
		delete spriteTexture;
	}

	void Application::OnWindowResize(int width, int height) {
		if (width && height) {
			engine->SetViewport(width, height);
			camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
			frameBuffer->SetSize(width, height);
			inverseWindowResolution = { 1.f / width, 1.f / height };
		}
	}

	void Application::OnKeyPress(int key, int mods) {
		switch (key) {
		case KEY_ESCAPE:
			Window::ToggleCursor();
			break;
		case KEY_F11:
			Window::ToggleFullscreen();
			break;
		}
	}
}