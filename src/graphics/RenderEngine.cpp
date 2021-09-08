#include "graphics/RenderEngine.hpp"
#include "graphics/opengl/GLRenderEngine.hpp"
//#include "graphics/vulkan/VKRenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Render Engine Interface
	//========================================================================

	API activeAPI = API::None;
	RenderEngine* engine = nullptr;

	RenderEngine* RenderEngine::Get() {
		DEBUG_ENSURE(engine, "The Render Engine has not yet been initialized. You must create a window before retrieving the render engine.");
		return engine;
	}

	void RenderEngine::Init() {
		DEBUG_ENSURE(activeAPI != API::None, "The Render Engine cannot be intialized before the Render API is set with RenderEngine::SetApi.");

		// This ensures that if multiple windows are created this operation is not repeated.
		if (!engine) {
			switch (activeAPI) {
			case API::OpenGL:
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
		DEBUG_ENSURE(activeAPI == API::None, "The render API has already been set - this operation cannot be performed multiple times.");
		activeAPI = api;
	}

	void RenderEngine::Draw(Mesh* m, unsigned int offset) {
		if (m->IsIndexed())
			DrawIndexed(m->GetCount(), offset);
		else
			DrawLinear(m->GetCount(), offset);
	}
}