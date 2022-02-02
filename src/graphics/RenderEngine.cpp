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

	void RenderEngine::Init() {
		if (engine)
			return;

		switch (activeAPI) {
		case API::OPENGL:
			engine = new GLRenderEngine();
			break;
		default:
			throw ZORE_EXCEPTION("The Render Engine cannot be intialized before the Render API is set with RenderEngine::SetApi.");
		}
	}

	RenderEngine* RenderEngine::Get() {
		return engine;
	}

	API RenderEngine::GetAPI() {
		return activeAPI;
	}

	void RenderEngine::SetAPI(API api) {
		DEBUG_ENSURE(activeAPI == API::NONE, "The render API has already been set - this operation cannot be performed multiple times.");
		DEBUG_ENSURE(api != API::NONE, "Cannot set the Render API to None");
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