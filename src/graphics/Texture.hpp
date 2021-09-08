#pragma once
#include <string>

namespace zore {

	class Texture2D {
	public:
		Texture2D* Create(const std::string& name);
		Texture2D* Create(unsigned int width, unsigned int height, unsigned int channels = 4);
		virtual ~Texture2D() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(unsigned int width, unsigned int height) = 0;
	};

}