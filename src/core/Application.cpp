#include "core/Application.hpp"
#include "graphics/VertexLayout.hpp"
#include "utils/ConfigManager.hpp"
#include "utils/FileManager.hpp"
#include "utils/Timer.hpp"
#include "debug/Debug.hpp"

#include "voxel/ChunkManager.hpp"
#include "voxel/World.hpp"
#include "game/Player.hpp"
#include "generation/TerrainGenerator.hpp"

#include "ui/Text.hpp"

namespace zore {

	ConfigGroup options("options");

	void Application::Init() {
		FileManager::Init();
		// load config from file
		options.Load();
		RenderEngine::SetAPI(API::OPENGL);
		Window::Init(options.Get("width", 0), options.Get("height", 0));
		RenderEngine::Init();

		Application app;
		app.Run();
		options.Save();
	}

	void Application::Cleanup() {
		ChunkManager::Cleanup();
		Window::Cleanup();
	}

	Application::Application() : engine(RenderEngine::Get()), camera(75.f, Window::GetAspectRatio(), 0.1f, 1200.f) {
		Window::SetBorderless(options.Get("borderless", false));
		Window::SetFullscreen(options.Get("fullscreen", false));
		Window::HideCursor(true);
		glm::ivec2 res = Window::GetSize();
		staticShaderData = { {1.f / res.x, 1.f / res.y} };
		dynamicShaderData = {};

		// Set initial settings for render engine
		engine->SetVSync(false);
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
		postProcessShader->SetTextureSlot("depth", 1);
		debugLineShader->Bind();
		debugLineShader->SetTextureSlot("depth", 1);
		spriteShader->Bind();
		spriteShader->SetTextureSlot("sprites", 3);
		textShader->Bind();
		textShader->SetTextureSlot("glyphs", 2);
		Font::SetTextureSlot(2);

		// Initialize Uniform Buffers for shaders
		dynamicShaderDataBuffer = UniformBuffer::Create(nullptr, sizeof(dynamicShaderData), BufferMode::DYNAMIC);
		dynamicShaderDataBuffer->Bind(0);
		staticShaderDataBuffer = UniformBuffer::Create(&staticShaderData, sizeof(staticShaderData), BufferMode::STATIC);
		staticShaderDataBuffer->Bind(1);
		glm::ivec4 offsetData[] = {
			{ 0, 1, 0, 0}, { 0, 0, 0, 0}, { 0, 1, 1, 0}, { 0, 0, 1, 0}, // -x WEST
			{ 1, 1, 1, 0}, { 1, 0, 1, 0}, { 1, 1, 0, 0}, { 1, 0, 0, 0}, // +x EAST
			{ 0, 0, 1, 0}, { 0, 0, 0, 0}, { 1, 0, 1, 0}, { 1, 0, 0, 0}, // -y DOWN
			{ 0, 1, 0, 0}, { 0, 1, 1, 0}, { 1, 1, 0, 0}, { 1, 1, 1, 0}, // +y UP
			{ 1, 1, 0, 0}, { 1, 0, 0, 0}, { 0, 1, 0, 0}, { 0, 0, 0, 0}, // -z NORTH
			{ 0, 1, 1, 0}, { 0, 0, 1, 0}, { 1, 1, 1, 0}, { 1, 0, 1, 0}, // +z SOUTH
			{ 0, 0, 0, 0}, { 0, 1, 0, 0}, { 1, 0, 1, 0}, { 1, 1, 1, 0}, // Sprite A
			{ 0, 0, 1, 0}, { 0, 1, 1, 0}, { 1, 0, 0, 0}, { 1, 1, 0, 0}, // Sprite B
		};
		offsetDataBuffer = UniformBuffer::Create(&offsetData, sizeof(offsetData), BufferMode::STATIC);
		offsetDataBuffer->Bind(2);

		// Create framebuffer
		frameBuffer = FrameBuffer::Create(res.x, res.y, 1, TextureFormat::RGBA, DepthFormat::DEPTH32_TEXTURE);
		frameBuffer->GetTextureArray()->Bind(0);
		frameBuffer->GetDepthTexture()->Bind(1);

		// Create Texture samplers and bind them to their respective texture units
		linearSampler = Sampler::Create(SampleMode::LINEAR);
		linearSampler->Bind(0);
		linearSampler->Bind(1);
		linearSampler->Bind(2);
		nearestSampler = Sampler::Create(SampleMode::NEAREST);
		nearestSampler->Bind(3);
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

		delete dynamicShaderDataBuffer;
		delete staticShaderDataBuffer;
		delete offsetDataBuffer;

		delete engine;
	}

	void Application::Run() {
		bool wireframe = false;
		// Create Textboxes
		//Font f("consola");
		//Textbox t(300, 100, "Sample Text");
		//Textbox b(300, 300, "Poggers");
		//Textbox::Flush();
		//t.SetText("Mom get the camera!");
		//Textbox::Flush();

		// Create mesh data for debug of currently selected voxel
		VertexLayout* UBx3 = VertexLayout::Create(debugLineShader, { {"pos", VertexDataType::UBYTE, 3} });
		ubyte lineVertices[] = { 0,0,0, 1,0,0, 0,0,1, 1,0,1, 0,1,0, 1,1,0, 0,1,1, 1,1,1, };
		Index lineIndices[] = { 0,1, 0,2, 1,3, 2,3, 0,4, 1,5, 2,6, 3,7, 4,5, 4,6, 5,7, 6,7 };
		Mesh* debugCubeMesh = Mesh::Create(&lineVertices, sizeof(ubyte) * 3, 8, &lineIndices, 24);
		debugCubeMesh->Bind();

		// Create the mesh used for screenspace rendering and voxel faces
		VertexLayout* UBx1 = VertexLayout::Create(blockShader, {}, { {"face", VertexDataType::UINT, 2} }, 1u);
		UBx1->Bind();

		// Create texture array for sprites;
		Texture2DArray* spriteTexture = Texture2DArray::Create({ "grass.png", "mush1.png" }, "assets/textures/", TextureFormat::RGBA);
		spriteTexture->Bind(3);

		// Initialize the player and chunk manager
		Player player(&camera, { 32, 128, 32 });
		TerrainGenerator::Init(123);
		ChunkManager::Init(options.Get("RenderDistance", 8), camera.GetPosition(), TerrainGenerator::Generate);

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
			dynamicShaderData = { camera.GetProjection() * camera.GetView(), inv_vp_mat, camera.GetPosition(), runningTime };
			dynamicShaderDataBuffer->Set(&dynamicShaderData, sizeof(dynamicShaderData));
			//dynamicShaderBuffer->Update(&dynamicShaderData, sizeof(dynamicShaderData));

			// RENDER THE SCENE TO FRAMEBUFFER --------
			frameBuffer->Bind();
			if (wireframe)
				engine->SetWireframe(true);
			engine->SetDepthTest(true);
			engine->Clear();

			// Render terrain and generic blocks
			blockShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				blockShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(4, chunk->GetChunkMesh(ChunkMeshType::BLOCKS).Bind());
			}
			// Render sprites (plants)
			engine->SetBackFaceCulling(false);
			spriteShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				spriteShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(4, chunk->GetChunkMesh(ChunkMeshType::SPRITES).Bind());
			}
			// Render fluids
			fluidShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				fluidShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(4, chunk->GetChunkMesh(ChunkMeshType::FLUIDS).Bind());
			}

			// Render Sky
			skyShader->Bind();
			engine->DrawLinear(4);
			engine->SetDepthTest(false);

			// Render debug for block that is currently selected
			glm::ivec3 pos;
			if (World::RaycastBlock(camera.GetPosition(), camera.GetForward(), pos, 10)) {
				engine->SetTopology(MeshTopology::LINE_LIST);
				UBx3->Bind();
				debugLineShader->Bind();
				debugLineShader->SetInt3("offset", pos);
				engine->DrawIndexed(debugCubeMesh->GetCount());
				// Rebind quad layout and mesh
				engine->SetTopology(MeshTopology::TRIANGLE_STRIP);
				UBx1->Bind();
			}
			engine->SetBackFaceCulling(true);

			// RENDER FRAMEBUFFER TEXTURE TO SCREEN --------
			frameBuffer->Unbind();
			if (wireframe)
				engine->SetWireframe(false);
			postProcessShader->Bind();
			engine->DrawLinear(4);

			// RENDER User Interface --------
			//textShader->Bind();
			//engine->DrawLinearInstanced(quadMesh->GetCount(), Textbox::Bind());

			Window::Update();
		}
		
		delete debugCubeMesh;
		delete UBx1;
		delete UBx3;
		delete spriteTexture;
	}

	void Application::OnWindowResize(int width, int height) {
		if (width && height) {
			engine->SetViewport(width, height);
			camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
			frameBuffer->SetSize(width, height);
			staticShaderData.inv_res = { 1.f / width, 1.f / height };
			staticShaderDataBuffer->Set(&staticShaderData, sizeof(staticShaderData));
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
		case KEY_F5:
			ChunkManager::SetRenderDistance(4);
			break;
		case KEY_F6:
			ChunkManager::SetRenderDistance(16);
			break;
		}
	}
}