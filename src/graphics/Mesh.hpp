#pragma once
#include "graphics/Buffer.hpp"
#include <string>

namespace zore {

	//========================================================================
	//  Platform Agnostic Mesh Class
	//========================================================================

	class Mesh {
	public:
		static Mesh* Create(void* vertices, unsigned int size, unsigned int stride);
		static Mesh* Create(void* vertices, unsigned int size, const std::string& layout);
		static Mesh* Create(void* vertices, unsigned int vsize, unsigned int stride, void* indices, unsigned int isize);
		static Mesh* Create(void* vertices, unsigned int vsize, void* indices, unsigned int isize, const std::string& layout);
		
		virtual ~Mesh();

		void Bind() const;
		void Unbind() const;
		void SetCount(unsigned int count);
		unsigned int GetCount() const;
		bool IsIndexed() const;

	protected:
		Mesh(unsigned int count, VertexBuffer* vb, IndexBuffer* ib);
		unsigned int count;
		VertexBuffer* vb;
		IndexBuffer* ib;
	};
}