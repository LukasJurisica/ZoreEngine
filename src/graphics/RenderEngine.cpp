#include "graphics/RenderEngine.hpp"
#include "graphics/opengl/GLRenderEngine.hpp"
//#include "graphics/vulkan/GLRenderEngine.hpp"
#include "debug/Debug.hpp"


namespace zore {

	//========================================================================
	//	Platform Agnostic Render Engine Interface
	//========================================================================

	API activeAPI = API::None;
	bool initialized = false;

	RenderEngine* RenderEngine::Create() {
		switch (activeAPI) {
		case API::OpenGL:
			return new GLRenderEngine();
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	void RenderEngine::Init() {
		if (!initialized)
			initialized = VerificaitonInit();
	}

	API RenderEngine::GetApi() {
		return activeAPI;
	}

	void RenderEngine::SetApi(API api) {
		activeAPI = api;
		initialized = false;
	}

	void RenderEngine::Draw(Mesh* m) {
		if (m->IsIndexed())
			DrawIndexed(m->GetCount());
		else
			DrawLinear(m->GetCount());
	}

	bool RenderEngine::VerificaitonInit() {
		switch (activeAPI) {
		case API::OpenGL:
			GLRenderEngine::Init();
			return true;
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return false;
	}
}