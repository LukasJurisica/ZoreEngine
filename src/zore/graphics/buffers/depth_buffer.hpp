#pragma once

#include "zore/graphics/graphics_core.hpp"
#include <string>

namespace zore {

	//========================================================================
	//	Depth Buffer
	//========================================================================

	enum class DepthFormat { DEPTH32_BUFFER, DEPTH24_STENCIL8_BUFFER, DEPTH32_TEXTURE, DEPTH24_STENCIL8_TEXTURE, NONE = -1 };

	class DepthBuffer {
	public:
		friend class FrameBuffer;

	private:
		DepthBuffer();
		DepthBuffer(uint32_t width, uint32_t height, DepthFormat format);
		DepthBuffer(const DepthBuffer&) = delete;
		DepthBuffer(DepthBuffer&&) = delete;
		DepthBuffer& operator=(const DepthBuffer&) = delete;
		DepthBuffer& operator=(DepthBuffer&&) = delete;
		~DepthBuffer();

	private:
		uint32_t GetInternalFormat();
		uint32_t GetBaseFormat();

	public:
		uint32_t GetID() const;
		void Set(uint32_t width, uint32_t height);
		void Set(uint32_t width, uint32_t height, DepthFormat format);
		void Bind() const;
		void Bind(uint32_t slot);
		void Bind(const std::string& slot);
		bool IsRenderBuffer() const;
		bool IsRenderTexture() const;

	private:
		uint32_t m_id;
		uint32_t m_slot;
		DepthFormat m_format;
	};
}