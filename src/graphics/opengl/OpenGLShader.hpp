#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "graphics/Shader.hpp"

class OpenGLShaderProgram : public Shader {
public:
	OpenGLShaderProgram(const std::string& filepath);
	~OpenGLShaderProgram();

	void bind();

	void setUniformInt(const char* name, int value);
	void setUniformFloat(const char* name, float value);
	void setUniformFloat2(const char* name, const glm::vec2& value);
	void setUniformFloat3(const char* name, const glm::vec3& value);
	void setUniformFloat4(const char* name, const glm::vec4& value);
	void setUniformMat4(const char* name, const glm::mat4& value);

private:
	static unsigned int shaderTypeFromString(const std::string& type);
	std::vector<unsigned int> preProcess(const std::string& filepath);
	void verifyUniformLocation(const char* name);
	void setupShader(const std::string& source, unsigned int shader);
	void setupProgram();

	//std::unordered_map<std::string, unsigned int> attributes;
	std::unordered_map<const char*, int> uniforms;
	int status;
	uint32_t programID;
};