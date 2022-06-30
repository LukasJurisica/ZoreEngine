#include "graphics/RenderEngine.hpp"
#include "graphics/opengl/GLRenderEngine.hpp"
//#include "graphics/vulkan/VKRenderEngine.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
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
			stbi_set_flip_vertically_on_load(true);
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

	//========================================================================
	//	Platform Agnostic Multidraw Command Buffer Class
	//========================================================================

	MultidrawCommand::MultidrawCommand(unsigned int vertexCount, unsigned int instanceCount, unsigned int vertexOffset, unsigned int instanceOffset) :
		vertexCount(vertexCount), instanceCount(instanceCount), vertexOffset(vertexOffset), instanceOffset(instanceOffset) {}

	MultidrawCommandBuffer* MultidrawCommandBuffer::Create(MultidrawCommand* data, unsigned int count, bool calculateInstanceOffsets) {
		if (calculateInstanceOffsets)
			CalculateInstanceOffsets(data, count, 0);
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLMultidrawCommandBuffer(data, count);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	void MultidrawCommandBuffer::CalculateInstanceOffsets(MultidrawCommand* data, unsigned int count, unsigned int baseOffset) {
		unsigned int offset = baseOffset;
		for (unsigned int i = 0; i < count; i++) {
			data[i].instanceOffset = offset;
			offset += data[i].instanceCount;
		}
	}
}