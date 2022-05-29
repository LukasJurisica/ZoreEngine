#pragma once
#include "graphics/Sampler.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Texture Sampler Class
	//========================================================================

	class GLSampler : public Sampler {
	public:
		GLSampler(SampleMode mode);
		~GLSampler();

		void Bind() const override;
		void Bind(unsigned int slot) override;
		void Unbind() const override;

	private:
		unsigned int id;
		unsigned int unit;
	};
}