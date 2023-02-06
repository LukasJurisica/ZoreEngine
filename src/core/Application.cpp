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
#include "math/Spline.hpp"

namespace zore {

	ConfigGroup options("options");

	void Application::Init() {
		FileManager::Init();
		// load config from file
		options.Load();
		RenderEngine::SetAPI(API::OPENGL);
		Window::Init(options.Get("width", 0), options.Get("height", 0));
		RenderEngine::Init();

		// Testing Spline
		zm::Spline s({ {-1, 2}, {0, 0}, {1, -2}, {2, 0} });

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
		staticShaderData = { {1.f / res.x, 1.f / res.y}, camera.GetNearDist(), camera.GetFarDist() };
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
		Texture::SetTextureSlot("screen", 0);
		Texture::SetTextureSlot("depth", 1);
		Texture::SetTextureSlot("glyphs", 2);
		Texture::SetTextureSlot("sprites", 3);

		postProcessShader->Bind();
		postProcessShader->SetTextureSlot("screen", Texture::GetTextureSlot("screen"));
		postProcessShader->SetTextureSlot("depth", Texture::GetTextureSlot("depth"));
		debugLineShader->Bind();
		debugLineShader->SetTextureSlot("depth", Texture::GetTextureSlot("depth"));
		spriteShader->Bind();
		spriteShader->SetTextureSlot("sprites", Texture::GetTextureSlot("sprites"));
		textShader->Bind();
		textShader->SetTextureSlot("glyphs", Texture::GetTextureSlot("glyphs"));

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
		frameBuffer = FrameBuffer::Create(res.x, res.y, 1, Texture::Format::RGBA, DepthFormat::DEPTH32_TEXTURE);
		frameBuffer->GetTextureArray()->Bind(Texture::GetTextureSlot("screen"));
		frameBuffer->GetDepthTexture()->Bind(Texture::GetTextureSlot("depth"));

		// Create Texture samplers and bind them to their respective texture units
		linearSampler = Texture::Sampler::Create(Texture::SampleMode::LINEAR);
		linearSampler->Bind(Texture::GetTextureSlot("screen"));
		linearSampler->Bind(Texture::GetTextureSlot("depth"));
		linearSampler->Bind(Texture::GetTextureSlot("glyphs"));
		nearestSampler = Texture::Sampler::Create(Texture::SampleMode::NEAREST);
		nearestSampler->Bind(Texture::GetTextureSlot("sprites"));
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
		if (wireframe)
			engine->SetClearMode({ BufferType::DEPTH, BufferType::COLOUR });
		// Create Textboxes
		//Font font("consola");
		//font.GetTextureArray()->Bind(2);
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

		// Create the Vertex layout for rendering the screen and block faces
		VertexLayout* UIx2 = VertexLayout::Create(blockShader, {}, { {"face", VertexDataType::UINT, 2} }, 1u);
		UIx2->Bind();

		// Create texture array for sprites;
		Texture2DArray* spriteTexture = Texture2DArray::Create({ "grass.png", "mush1.png" }, "assets/textures/", Texture::Format::RGBA);
		spriteTexture->Bind(Texture::GetTextureSlot("sprites"));

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
				//Logger::Log("FrameRate: " + std::to_string(frameCount));
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
			if (wireframe) {
				engine->SetWireframe(true);
				engine->Clear();
			}
			engine->SetDepthTest(true);
			engine->Clear();

			// Render terrain and generic blocks
			blockShader->Bind();
			for (const Chunk* chunk : ChunkManager::GetVisibleChunks()) {
				blockShader->SetInt3("chunkPos", chunk->GetPosition());
				engine->DrawLinearInstanced(4, chunk->GetChunkMesh(ChunkMeshType::BLOCKS).Bind());
			}
			// Render sprites (plants)
			engine->SetFaceCulling(CullingMode::NONE);
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
				// Rebind quad layout and topolgy
				engine->SetTopology(MeshTopology::TRIANGLE_STRIP);
				UIx2->Bind();
			}
			engine->SetFaceCulling(CullingMode::BACK);

			// RENDER FRAMEBUFFER TEXTURE TO SCREEN --------
			frameBuffer->Unbind();
			if (wireframe)
				engine->SetWireframe(false);
			postProcessShader->Bind();
			engine->DrawLinear(4);

			// RENDER User Interface --------
			//textShader->Bind();
			//engine->DrawLinearInstanced(4, Textbox::Bind());

			Window::Update();
		}
		
		delete debugCubeMesh;
		delete UIx2;
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