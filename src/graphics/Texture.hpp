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

	//========================================================================
	//	Platform Agnostic 2D Texture Class
	//========================================================================

	class Texture2D {
	public:
		static Texture2D* Create(const std::string& name, TextureFormat textureFormat = TextureFormat::RGBA);
		static Texture2D* Create(uint width, uint height, TextureFormat textureFormat = TextureFormat::RGBA, void* data = nullptr);
		virtual ~Texture2D() = default;

		virtual void Bind() const = 0;
		virtual void Bind(unsigned int slot) = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(uint width, uint height) = 0;
		virtual void SetTextureData(void* data) = 0;

	protected:
		void Init(uint width, uint height);
		uint width = 0, height = 0;
	};

	//========================================================================
	//	Platform Agnostic 2D Array Texture Class
	//========================================================================
	
	class Texture2DArray {
	public:
		static Texture2DArray* Create(const std::vector<std::string>& filenames, const std::string& root = "assets/", TextureFormat textureFormat = TextureFormat::RGBA);
		static Texture2DArray* Create(uint width, uint height, uint layers, void* data = nullptr, TextureFormat textureFormat = TextureFormat::RGBA);
		virtual ~Texture2DArray() = default;

		virtual void Bind() const = 0;
		virtual void Bind(unsigned int slot) = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(uint width, uint height, uint layers) = 0;
		virtual void SetTextureData(uint first, uint count, void* data) = 0;

	protected:
		void Init(uint width, uint height, uint layers);
		uint width = 0, height = 0, layers = 0;
	};
}