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
		enum class Format { R, RG, RGB, RGBA, RU, RGU, RGBU, RGBAU,  };

	protected:
		Texture(uint32_t target, Format format);
		Texture(const Texture&) = delete;
		Texture(Texture&&) noexcept;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) noexcept;
		void Swap(Texture& other);
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
		uint32_t m_target;
		uint32_t m_slot;
	};

	//========================================================================
	//	2D Texture Class
	//========================================================================
	
	class Texture2D : public Texture {
	public:
		Texture2D(Texture::Format format = Texture::Format::RGBA);
		Texture2D(void* data, uint32_t width, uint32_t height, Texture::Format format = Texture::Format::RGBA);
		Texture2D(const std::string& filename, Texture::Format format = Texture::Format::RGBA);
		Texture2D(Texture2D&&) noexcept;
		Texture2D& operator=(Texture2D&&) noexcept;
		~Texture2D() = default;

		void Update(void* data);
		void Set(void* data, uint32_t width, uint32_t height);
		void Set(void* data, uint32_t width, uint32_t height, Texture::Format format);
		void Set(const std::string& filename, Texture::Format format = Texture::Format::RGBA);

		uint32_t GetWidth() { return m_width; }
		uint32_t GetHeight() { return m_height; }

	private:
		void Swap(Texture2D& other);

	private:
		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};
	
	//========================================================================
	//  2D Array Texture Class
	//========================================================================
	
	class Texture2DArray : public Texture {
	public:
		Texture2DArray(Texture::Format format = Texture::Format::RGBA);
		Texture2DArray(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format = Texture::Format::RGBA);
		Texture2DArray(const std::vector<std::string>& filenames, const std::string& root = "assets/", Texture::Format format = Texture::Format::RGBA);
		Texture2DArray(Texture2DArray&&) noexcept;
		Texture2DArray& operator=(Texture2DArray&&) noexcept;
		~Texture2DArray() = default;

		void Update(void* data, uint32_t offset = 0, uint32_t count = ~0);
		void Set(void* data, uint32_t width, uint32_t height, uint32_t layers);
		void Set(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format);
		void Set(const std::vector<std::string>& filenames, const std::string& root = "assets/", Texture::Format format = Texture::Format::RGBA);

		uint32_t GetWidth() { return m_width; }
		uint32_t GetHeight() { return m_height; }
		uint32_t GetCount() { return m_layers; }

	private:
		void Swap(Texture2DArray& other);

	private:
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_layers = 0;
	};
}