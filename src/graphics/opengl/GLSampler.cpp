#include "graphics/opengl/GLSampler.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Texture Sampler Class
	//========================================================================

	static const unsigned int SamplerModeToGLMode[] = { GL_LINEAR, GL_NEAREST };

	GLSampler::GLSampler(SampleMode mode) : unit(0) {
		glCreateSamplers(1, &id);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(id, GL_TEXTURE_MIN_LOD, 0);
		glSamplerParameteri(id, GL_TEXTURE_MAX_LOD, 0);
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, SamplerModeToGLMode[static_cast<int>(mode)]); // Close up Voxel
		//glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, SamplerModeToGLMode[static_cast<int>(mode)]); // Far away voxel
	}

	GLSampler::~GLSampler() {
		glDeleteSamplers(1, &id);
	}

	void GLSampler::Bind() const {
		glBindSampler(unit, id);
	}

	void GLSampler::Bind(unsigned int slot) {
		unit = slot;
		glBindSampler(unit, id);
	}

	void GLSampler::Unbind() const {
		glBindSampler(unit, 0);
	}
}