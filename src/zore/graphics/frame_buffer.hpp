#pragma once

#include "zore/graphics/graphics_core.hpp"
#include "zore/graphics/textures/texture_2D_array.hpp"
#include "zore/graphics/buffers/depth_buffer.hpp"
#include <vector>

namespace zore {

	//========================================================================
	//	Frame Buffer
	//========================================================================

	class FrameBuffer {
	public:
		FrameBuffer();
		FrameBuffer(uint32_t width, uint32_t height, uint32_t a_count, Texture::Format a_format = Texture::Format::RGBA, DepthFormat d_format = DepthFormat::DEPTH32_BUFFER);
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer& operator=(FrameBuffer&&) = delete;
		~FrameBuffer();

		uint32_t GetID() const;
		Texture2DArray& GetColourTexture();
		DepthBuffer& GetDepthBuffer();
		void Set(uint32_t width, uint32_t height);
		void Set(uint32_t width, uint32_t height, uint32_t a_count, Texture::Format a_format = Texture::Format::RGBA, DepthFormat d_format = DepthFormat::DEPTH32_BUFFER);
		void Bind() const;
		static void Unbind();

	private:
		void UpdateAttachments();

	private:
		uint32_t m_id;
		uint32_t m_attachment_count;
		Texture2DArray m_colour_buffer;
		DepthBuffer m_depth_buffer;
	};
}