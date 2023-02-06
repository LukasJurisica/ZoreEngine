#pragma once
#include "utils/DataTypes.hpp"
#include <vector>
#include <string>

namespace zore {

	//========================================================================
	//	Platform Agnostic Texture Utility
	//========================================================================

	class Texture {
	public:
		static void SetTextureSlot(const std::string& name, unsigned int slot);
		static unsigned int GetTextureSlot(const std::string& name);
		
		enum class Format { R, RG, RGB, RGBA };

		//========================================================================
		//	Platform Agnostic Texture Sampler Class
		//========================================================================

		enum class SampleMode { LINEAR, NEAREST };
		enum class WrapMode { CLAMP, REPEAT };

		class Sampler {
		public:
			static Sampler* Create(SampleMode mode);
			virtual ~Sampler() = default;

			void Bind(const std::string& slot);
			virtual void Bind() const = 0;
			virtual void Bind(unsigned int slot) = 0;
			virtual void Unbind() const = 0;
		};
	};

	//========================================================================
	//	Platform Agnostic 2D Texture Class
	//========================================================================

	class Texture2D {
	public:
		static Texture2D* Create(const std::string& filename, Texture::Format format = Texture::Format::RGBA);
		static Texture2D* Create(uint width, uint height, Texture::Format format = Texture::Format::RGBA, void* data = nullptr);
		virtual ~Texture2D() = default;

		void Bind(const std::string& slot);
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
		static Texture2DArray* Create(const std::vector<std::string>& filenames, const std::string& root = "assets/", Texture::Format format = Texture::Format::RGBA);
		static Texture2DArray* Create(uint width, uint height, uint layers, Texture::Format format = Texture::Format::RGBA, void* data = nullptr);
		virtual ~Texture2DArray() = default;

		void Bind(const std::string& slot);
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