#pragma once

#include <cstdint>
#include <vector>
#include "graphics/Shader.hpp"

class OpenGLShaderProgram : public Shader {
public:
	OpenGLShaderProgram(const std::string& filename);
	~OpenGLShaderProgram();

	void bind();

	void setUniformInt(const char* name, int value);
	void setUniformFloat(const char* name, float value);
	void setUniformFloat2(const char* name, const glm::vec2& value);
	void setUniformFloat3(const char* name, const glm::vec3& value);
	void setUniformFloat4(const char* name, const glm::vec4& value);
	void setUniformMat4(const char* name, const glm::mat4& value);

private:
	std::vector<unsigned int> process(const std::string& source);
	void setupShader(const std::string& source, unsigned int shader);
	void setupProgram();

	int status;
	uint32_t programID;
};