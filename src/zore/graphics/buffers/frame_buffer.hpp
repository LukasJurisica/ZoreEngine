#pragma once

namespace zore {

	//========================================================================
	//	Frame Buffer
	//========================================================================

	class FrameBuffer {
	public:
		FrameBuffer();
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer& operator=(FrameBuffer&&) = delete;
		~FrameBuffer();

	private:

	};
}