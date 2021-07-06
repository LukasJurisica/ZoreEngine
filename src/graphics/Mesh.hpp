#pragma once
#include <string>

namespace zore {

	//========================================================================
	//  Platform Agnostic Mesh Class
	//========================================================================

	enum class MeshTopology { TriangleList, TriangleStrip, TriangleFan };

	class Mesh {
	public:
		static Mesh* Create(void* vertices, unsigned int vsize, void* indices, unsigned int isize, const std::string& layout);
		static Mesh* Create(void* vertices, unsigned int size, const std::string& layout);
		virtual ~Mesh() = default;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		unsigned int GetCount() const;
		bool IsIndexed() const;

	protected:
		Mesh(unsigned int count, bool indexed);

		unsigned int count = 0;
		bool indexed;
	};
}