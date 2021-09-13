#include "graphics/RenderEngine.hpp"
#include "graphics/opengl/GLRenderEngine.hpp"
//#include "graphics/vulkan/VKRenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Render Engine Interface
	//========================================================================

	API activeAPI = API::NONE;
	RenderEngine* engine = nullptr;

	RenderEngine* RenderEngine::Get() {
		DEBUG_ENSURE(engine, "The Render Engine has not yet been initialized. You must create a window before retrieving the render engine.");
		return engine;
	}

	void RenderEngine::Init() {
		DEBUG_ENSURE(activeAPI != API::NONE, "The Render Engine cannot be intialized before the Render API is set with RenderEngine::SetApi.");

		// This ensures that if multiple windows are created this operation is not repeated.
		if (!engine) {
			switch (activeAPI) {
			case API::OPENGL:
				engine = new GLRenderEngine();
				break;
			default:
				throw ZORE_EXCEPTION("Invalid RenderAPI");
			}
		}
	}

	API RenderEngine::GetAPI() {
		return activeAPI;
	}

	void RenderEngine::SetAPI(API api) {
		DEBUG_ENSURE(activeAPI == API::NONE, "The render API has already been set - this operation cannot be performed multiple times.");
		activeAPI = api;
	}

	void RenderEngine::Draw(Mesh* mesh, unsigned int offset) {
		mesh->Bind();
		if (mesh->IsIndexed())
			DrawIndexed(mesh->GetCount(), offset);
		else
			DrawLinear(mesh->GetCount(), offset);
	}

	void RenderEngine::DrawInstanced(Mesh* mesh, unsigned int count, unsigned int offset) {
		mesh->Bind();
		if (mesh->IsIndexed())
			DrawIndexedInstanced(mesh->GetCount(), count, offset);
		else
			DrawLinearInstanced(mesh->GetCount(), count, offset);
	}
}