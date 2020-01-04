#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

class Shader {
public:
	virtual ~Shader() {}

	virtual void bind() = 0;
	virtual void setUniformInt(const char* name, int value) = 0;
	virtual void setUniformFloat(const char* name, float value) = 0;
	virtual void setUniformFloat2(const char* name, const glm::vec2& value) = 0;
	virtual void setUniformFloat3(const char* name, const glm::vec3& value) = 0;
	virtual void setUniformFloat4(const char* name, const glm::vec4& value) = 0;
	virtual void setUniformMat4(const char* name, const glm::mat4& value) = 0;

	static Shader* create(const char* vertpath, const char* fragpath);
};