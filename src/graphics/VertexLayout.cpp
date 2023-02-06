#include "graphics/VertexLayout.hpp"
#include "graphics/opengl/GLVertexLayout.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"
#include <iostream>

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	const unsigned int VertexLayout::VertexDataTypeSize[] = { 1u, 1u, 1u, 4u, 4u, 4u };

	VertexElement::VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize) :
		type(type), count(count), name(name), normalize(normalize) {
		ENSURE(count < 5 && count > 0, "Invalid number of components for BufferElement");
	}

	VertexLayout* VertexLayout::Create(Shader* shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements, unsigned int interval) {
		DEBUG_ENSURE(shader, "Failed to create vertex layout. Null shader pointer provided.");

		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLVertexLayout(shader, vertexElements, instanceElements, interval);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}
}