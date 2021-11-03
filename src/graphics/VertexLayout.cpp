#include "graphics/VertexLayout.hpp"
#include "graphics/opengl/GLVertexLayout.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	std::vector<VertexLayout*> layouts;
	const unsigned int VertexLayout::VertexDataTypeSize[] = { 1u, 1u, 1u, 4u, 4u, 4u };

	VertexElement::VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize) :
		type(type), count(count), name(name), normalize(normalize) {
		ENSURE(count < 5 && count > 0, "Invalid number of components for BufferElement");
	}

	VertexLayout::VertexLayout(const std::string& name) : name(name) {
		layouts.push_back(this);
	}

	VertexLayout* VertexLayout::Create(const std::string& name, Shader* shader, const std::vector<VertexElement>& elements) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLVertexLayout(name, shader, elements, {}, 1);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	VertexLayout* VertexLayout::Create(const std::string& name, Shader* shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements, unsigned int interval) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLVertexLayout(name, shader, vertexElements, instanceElements, interval);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	VertexLayout* VertexLayout::Get(const std::string& name) {
		for (VertexLayout* layout : layouts) {
			if (layout->name == name)
				return layout;
		}
		throw ZORE_EXCEPTION("No vertex buffer layout with the name: " + name);
	}

	VertexLayout::~VertexLayout() {
		auto iter = std::find(layouts.begin(), layouts.end(), this);
		if (iter != layouts.end())
			layouts.erase(iter);
	}
}