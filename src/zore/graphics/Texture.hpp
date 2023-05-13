#pragma once
#include "zore/utils/DataTypes.hpp"
#include <vector>
#include <string>

namespace zore {

	//========================================================================
	//	Texture Sampler Class
	//========================================================================

	class TextureSampler {
	public:
		TextureSampler();
		TextureSampler(const TextureSampler&) = delete;
		TextureSampler(TextureSampler&&) = delete;
		TextureSampler& operator=(const TextureSampler&) = delete;
		TextureSampler& operator=(TextureSampler&&) = delete;
		~TextureSampler();

		enum class Wrap { CLAMP, REPEAT };
		enum class Filter { LINEAR, NEAREST };

		uint32_t GetID() const;
		TextureSampler& SetWrapMode(Wrap mode);
		TextureSampler& SetLODLevels(uint32_t min, uint32_t max);
		TextureSampler& SetFilters(Filter min, Filter mag);
		void Bind(uint32_t slot) const;
		void Bind(std::vector<uint32_t> slots) const;
		void Bind(const std::string& slot) const;
		void Bind(std::vector<std::string> slots) const;

	private:
		uint32_t m_id;
	};

	//========================================================================
	//	Texture Base Class
	//========================================================================
		
	class Texture {
	public:
		enum class Format { R, RG, RGB, RGBA };

	protected:
		Texture(uint32_t target, Format format);
		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;
		~Texture();

	public:
		uint32_t GetID() const;
		void Bind() const;
		void Bind(uint32_t slot);
		void Bind(const std::string& slot);

		static void SetNamedTextureSlot(const std::string& name, uint32_t slot);
		static uint32_t GetNamedTextureSlot(const std::string& name);

	protected:
		uint32_t m_id;
		uint32_t m_format;
		uint32_t m_slot;
	};

	//========================================================================
	//	2D Texture Class
	//========================================================================
	
	class Texture2D : public Texture {
	public:
		Texture2D();
		Texture2D(uint32_t width, uint32_t height, Texture::Format format = Texture::Format::RGBA);
		Texture2D(const std::string& filename, Texture::Format format = Texture::Format::RGBA);
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&&) = delete;
		~Texture2D() = default;

		void SetData(void* data);
		Texture2D& SetSize(uint32_t width, uint32_t height);
		Texture2D& SetSize(uint32_t width, uint32_t height, Texture::Format format);
		void Set(const std::string& filename, Texture::Format format = Texture::Format::RGBA);
	
	private:
		void Init(uint32_t width, uint32_t height);

	private:
		uint32_t m_width;
		uint32_t m_height;
	};
	
	//========================================================================
	//  2D Array Texture Class
	//========================================================================
	
	class Texture2DArray : public Texture {
	public:
		Texture2DArray();
		Texture2DArray(uint32_t width, uint32_t height, uint32_t layers, Texture::Format format = Texture::Format::RGBA);
		Texture2DArray(const std::vector<std::string>& filenames, const std::string& root = "assets/", Texture::Format format = Texture::Format::RGBA);
		Texture2DArray(const Texture2DArray&) = delete;
		Texture2DArray(Texture2DArray&&) = delete;
		Texture2DArray& operator=(const Texture2DArray&) = delete;
		Texture2DArray& operator=(Texture2DArray&&) = delete;
		~Texture2DArray() = default;

		void SetData(uint32_t first, uint32_t count, void* data);
		Texture2DArray& SetSize(uint32_t width, uint32_t height, uint32_t layers);
		Texture2DArray& SetSize(uint32_t width, uint32_t height, uint32_t layers, Texture::Format format);
		void Set(const std::vector<std::string>& filenames, const std::string& root = "assets/", Texture::Format format = Texture::Format::RGBA);

	private:
		void Init(uint32_t width, uint32_t height, uint32_t layers);

	private:
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_layers = 0;
	};
}