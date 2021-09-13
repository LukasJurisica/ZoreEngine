#pragma once
#include "utils/DataTypes.hpp"
#include <string>

namespace zore {

	//========================================================================
	//	Platform Agnostic Texture Utility
	//========================================================================

	enum class TextureFormat {
		R, RG, RGB, RGBA
	};

	class TextureSampler {
	public:

	};

	//========================================================================
	//	Platform Agnostic 2D Texture Class
	//========================================================================

	class Texture2D {
	public:
		static Texture2D* Create(const std::string& name);
		static Texture2D* Create(uint width, uint height, TextureFormat format = TextureFormat::RGBA);
		virtual ~Texture2D() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(uint width, uint height) = 0;
	};
}