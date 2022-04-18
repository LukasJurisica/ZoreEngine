#include "core/Application.hpp"
#include "graphics/VertexLayout.hpp"
#include "utils/FileManager.hpp"
#include "utils/DataTypes.hpp"
#include "utils/Timer.hpp"
#include "voxel/ChunkManager.hpp"
#include "debug/Debug.hpp"
#include "math/MathUtils.hpp"

#include "game/Player.hpp"
#include "game/World.hpp"

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

		engine->SetVSync(false);
		engine->SetClearColour(0.5f, 0.8f, 0.92f);
		engine->SetClearMode({ BufferType::COLOUR, BufferType::DEPTH });
		engine->SetTopology(MeshTopology::TRIANGLE_STRIP);

		// Compile Shaders
		blockShader = Shader::Create("VoxelBlocks");
		fluidShader = Shader::Create("VoxelFluids");
		spriteShader = Shader::Create("VoxelSprites");
		debugLineShader = Shader::Create("DebugLines");
		postProcessShader = Shader::Create("postprocess");

		// Set initial uniform values for shaders
		spriteShader->Bind();
		spriteShader->SetTextureSlot("spriteTextures", 1);
		postProcessShader->Bind();
		glm::ivec2 res = Window::GetSize();
		postProcessShader->SetFloat2("resolution", { 1.f / res.x, 1.f / res.y });
		postProcessShader->SetTextureSlot("screen", 0);

		frameBuffer = FrameBuffer::Create(WINDOW_SIZE, 1, TextureFormat::RGBA, DepthFormat::DEPTH24_STENCIL8);
		frameBuffer->GetTextureArray()->SetTextureSlot(0);
	}

	Application::~Application() {
		delete frameBuffer;
		delete postProcessShader;
		delete blockShader;
		delete fluidShader;
		delete spriteShader;
		delete engine;
	}

	void Application::Run() {
		// Create the Player and Camera uniform buffer
		Player player(&camera, { 32, 128, 32 });
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
		Mesh* quadMesh = Mesh::Create(&vertices, sizeof(vertices[0]), sizeof(vertices) / sizeof(vertices[0]));
		// Create mesh data for debug of currently selected voxel
		VertexLayout* UBx3 = VertexLayout::Create("UBx3", debugLineShader, { {"pos", VertexDataType::UBYTE, 3} });
		ubyte lineVertices[] = { 0,0,0, 1,0,0, 0,0,1, 1,0,1, 0,1,0, 1,1,0, 0,1,1, 1,1,1, };
		Index lineIndices[] = { 0,1, 0,2, 1,3, 2,3, 0,4, 1,5, 2,6, 3,7, 4,5, 4,6, 5,7, 6,7 };
		Mesh* debugCubeMesh = Mesh::Create(&lineVertices, sizeof(ubyte) * 3, 8, &lineIndices, 24);
		// Create texture array for sprites;
		Texture2DArray* spriteTexture = Texture2DArray::Create({ "assets/textures/grass.png", "assets/textures/mush1.png" }, TextureFormat::RGBA);
		spriteTexture->SetTextureSlot(1);

		// Initialize the chunk manager
		ChunkManager::Init(RENDER_DISTANCE, camera.GetPosition());

		//Bind mesh and vertex layouts for rendering
		engine->SetTopology(MeshTopology::TRIANGLE_STRIP);
		UBx1->Bind();
		quadMesh->Bind();

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
			shaderData = { camera.GetProjection() * camera.GetView(), camera.GetPosition(), runningTime };
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

			// Render debug for block that is currently selected
			engine->SetBackFaceCulling(true);
			engine->SetDepthTest(false);
			glm::ivec3 pos;
			if (World::RaycastBlock(camera.GetPosition(), camera.GetForward(), pos, 10)) {
				engine->SetTopology(MeshTopology::LINE_LIST);
				UBx3->Bind();
				debugCubeMesh->Bind();
				debugLineShader->Bind();
				debugLineShader->SetInt3("offset", { pos.x, pos.y, pos.z });
				engine->DrawIndexed(debugCubeMesh->GetCount());
				// Rebind quad layout and mesh
				engine->SetTopology(MeshTopology::TRIANGLE_STRIP);
				UBx1->Bind();
				quadMesh->Bind();
			}

			// RENDER FRAMEBUFFER TEXTURE TO SCREEN --------
			frameBuffer->Unbind();
			//engine->SetWireframe(false);
			postProcessShader->Bind();
			engine->DrawLinear(quadMesh->GetCount());
			Window::Update();
		}
		
		delete quadMesh;
		delete UBx1;
		delete UBx3;
		delete shaderDataBuffer;
		delete offsetDataBuffer;
	}

	void Application::OnWindowResize(int width, int height) {
		if (width && height) {
			engine->SetViewport(width, height);
			camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
			frameBuffer->SetSize(width, height);
			postProcessShader->Bind();
			postProcessShader->SetFloat2("resolution", { 1.f / width, 1.f / height });
		}
	}

	void Application::OnKeyPress(int key, int mods) {
		switch (key) {
		case KEY_ESCAPE:
			Window::ToggleCursor();
			break;
		}
	}
}