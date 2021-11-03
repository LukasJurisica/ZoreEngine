#pragma once
#include "graphics/Buffer.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//  Platform Agnostic Mesh Class
	//========================================================================

	class Mesh {
	public:
		static Mesh* Create(const void* vertices, unsigned int stride, unsigned int count);
		static Mesh* Create(const void* vertices, unsigned int stride, unsigned int vCount, const void* indices, unsigned int iCount);
		virtual ~Mesh();

		void Bind() const;
		void Unbind() const;
		unsigned int GetCount() const;
		bool IsIndexed() const;

	protected:
		Mesh(unsigned int count, VertexBuffer* vb, IndexBuffer* ib);
		unsigned int count;
		VertexBuffer* vb;
		IndexBuffer* ib;
	};
}