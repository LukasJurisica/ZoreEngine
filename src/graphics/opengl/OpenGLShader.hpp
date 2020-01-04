#pragma once

#include "graphics/Shader.hpp"

class OpenGLShader : public Shader {
public:
	OpenGLShader(const char* vertPath, const char* fragPath);
	~OpenGLShader();

	void bind();

private:
	void setupShader(unsigned int shader, const char* filename);
	void setupProgram();

	void setUniformInt(const char* name, int value);
	void setUniformFloat(const char* name, float value);
	void setUniformFloat2(const char* name, const glm::vec2& value);
	void setUniformFloat3(const char* name, const glm::vec3& value);
	void setUniformFloat4(const char* name, const glm::vec4& value);
	void setUniformMat4(const char* name, const glm::mat4& value);

	int status;
	unsigned int programID;
};