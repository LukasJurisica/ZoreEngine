#pragma once

namespace zore {

	//========================================================================
	//	Platform Agnostic Texture Sampler Class
	//========================================================================

	enum class SampleMode {
		LINEAR, NEAREST
	};

	enum class WrapMode {
		CLAMP, REPEAT
	};

	class Sampler {
	public:
		static Sampler* Create(SampleMode mode);
		virtual ~Sampler() = default;

		virtual void Bind() const = 0;
		virtual void Bind(unsigned int slot) = 0;
		virtual void Unbind() const = 0;
	};
}