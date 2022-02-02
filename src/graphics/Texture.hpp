#pragma once
#include "utils/DataTypes.hpp"
#include <vector>
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
		Texture2D(uint width, uint height);
		static Texture2D* Create(const std::string& name, TextureFormat textureFormat = TextureFormat::RGBA);
		static Texture2D* Create(uint width, uint height, TextureFormat textureFormat = TextureFormat::RGBA, void* data = nullptr);
		virtual ~Texture2D() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetTextureSlot(unsigned int slot) = 0;

	protected:
		uint width, height;
	};

	//========================================================================
	//	Platform Agnostic 2D Array Texture Class
	//========================================================================
	
	class Texture2DArray {
	public:
		Texture2DArray(uint width, uint height, uint layers);
		static Texture2DArray* Create(uint width, uint height, const std::vector<std::string>& filenames, TextureFormat textureFormat = TextureFormat::RGBA);
		static Texture2DArray* Create(uint width, uint height, uint layers, void* data = nullptr, TextureFormat textureFormat = TextureFormat::RGBA);
		virtual ~Texture2DArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetTextureSlot(unsigned int slot) = 0;
		virtual void SetTextureData(uint first, uint count, void* data) = 0;

	protected:
		uint width, height, layers;
	};
}