#include "graphics/Sampler.hpp"
#include "graphics/opengl/GLSampler.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Texture Sampler Class
	//========================================================================

	Sampler* Sampler::Create(SampleMode mode) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLSampler(mode);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}
}