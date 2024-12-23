#pragma once
#include "zore/graphics/Texture.hpp"
#include "zore/utils/DataTypes.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//	Depth Buffer Class
	//========================================================================

	enum class DepthFormat { DEPTH32_BUFFER, DEPTH24_STENCIL8_BUFFER, DEPTH32_TEXTURE, DEPTH24_STENCIL8_TEXTURE, NONE };

	class DepthBuffer {
		friend class FrameBuffer;
	private:
		DepthBuffer();
		DepthBuffer(uint32_t width, uint32_t height, DepthFormat depthFormat);
		DepthBuffer(const DepthBuffer&) = delete;
		DepthBuffer(DepthBuffer&&) = delete;
		DepthBuffer& operator=(const DepthBuffer&) = delete;
		DepthBuffer& operator=(DepthBuffer&&) = delete;
		~DepthBuffer();

	public:
		uint32_t GetID() const;
		void SetSize(uint32_t width, uint32_t height);
		void Set(uint32_t width, uint32_t height, DepthFormat depthFormat);
		void Bind() const;
		void Bind(uint32_t slot);
		void Bind(const std::string& slot);

	private:
		void Free();

	private:
		uint32_t m_id;
		uint32_t m_slot;
		DepthFormat m_format;
	};

	//========================================================================
	//	Frame Buffer Class
	//========================================================================

	class FrameBuffer {
	public:
		FrameBuffer();
		FrameBuffer(uint32_t width, uint32_t height, uint32_t attachmentCount, Texture::Format attachmentFormat = Texture::Format::RGBA, DepthFormat depthFormat = DepthFormat::DEPTH32_BUFFER);
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer& operator=(FrameBuffer&&) = delete;
		~FrameBuffer();

		uint32_t GetID() const;
		Texture2DArray& GetColourTexture();
		DepthBuffer& GetDepthTexture();
		void SetSize(uint32_t width, uint32_t height);
		void Set(uint32_t width, uint32_t height, uint32_t attachmentCount, Texture::Format attachmentFormat = Texture::Format::RGBA, DepthFormat depthFormat = DepthFormat::DEPTH32_BUFFER);
		void Bind() const;
		static void Unbind();

	private:
		uint32_t m_id;
		uint32_t m_attachment_count;
		Texture2DArray m_colour_buffer;
		DepthBuffer m_depth_buffer;
	};
}