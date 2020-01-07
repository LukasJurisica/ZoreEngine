#include "graphics/opengl/OpenGLShader.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include "utils/Logger.hpp"
#include "utils/StringUtils.hpp"

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& filepath) {
	programID = glCreateProgram();
	std::vector<GLuint>& shaders = preProcess(filepath);

	if (shaders.size() != 2)
		Logger::error("Must have exactly 2 (a vertex and a fragment) shader");

	setupProgram();
	for (GLuint shader : shaders) {
		glDetachShader(programID, shader);
		glDeleteShader(shader);
	}
}

std::vector<GLuint> OpenGLShaderProgram::preProcess(const std::string& filepath) {
	std::vector<GLuint> shaders;

	std::ifstream f(filepath);
	if (f.fail())
		Logger::error("Error opening file: " + std::string(filepath));

	std::string line, source;
	std::vector<std::string> parts;
	GLenum type;
	GLuint shader;

	while (std::getline(f, line)) {
		parts = StringUtils::split(line, " ()=;");
		if (parts.size() == 0)
			continue;
		else if (parts[0] == "#type") {
			type = shaderTypeFromString(parts[1]);
			shader = glCreateShader(type);
			shaders.push_back(shader);
			setupShader(source, shader);
			source = "";
		}
		else
			source += line + "\n";
	}

	f.close();	
	return shaders;
}

void OpenGLShaderProgram::setupShader(const std::string& source, unsigned int shader) {
	const char* source_cstr = source.c_str();
	glShaderSource(shader, 1, &source_cstr, NULL);
	glCompileShader(shader);
	// Check if shaders compiled successfully
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
		char* buffer = new char[status];
		glGetShaderInfoLog(shader, status, NULL, buffer);
		glDeleteShader(shader);
		Logger::error("Shader compilation failed: " + std::string(buffer));
	}
	glAttachShader(programID, shader);
}

void OpenGLShaderProgram::setupProgram() {
	glLinkProgram(programID);
	// Check if shaders were linked successfully
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &status);
		char* buffer = new char[status];
		glGetProgramInfoLog(programID, status, NULL, buffer);
		glDeleteProgram(programID);
		Logger::error("Linking shaders failed: " + std::string(buffer));
	}
}

void OpenGLShaderProgram::bind() {
	glUseProgram(programID);
}

void OpenGLShaderProgram::setUniformInt(const char* name, int value) {

}

void OpenGLShaderProgram::setUniformFloat(const char* name, float value) {

}

void OpenGLShaderProgram::setUniformFloat2(const char* name, const glm::vec2& value) {

}

void OpenGLShaderProgram::setUniformFloat3(const char* name, const glm::vec3& value) {

}

void OpenGLShaderProgram::setUniformFloat4(const char* name, const glm::vec4& value) {

}

void OpenGLShaderProgram::setUniformMat4(const char* name, const glm::mat4& value) {
	verifyUniformLocation(name);
	glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShaderProgram::verifyUniformLocation(const char* name) {
	if (uniforms.find(name) == uniforms.end()) {
		GLint loc = glGetUniformLocation(programID, name);
		if (loc == -1)
			Logger::error("Uniform not found: " + std::string(name));
		uniforms[name] = loc;
	}
}

GLenum OpenGLShaderProgram::shaderTypeFromString(const std::string& type) {
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	else if (type == "fragment")
		return GL_FRAGMENT_SHADER;
	else
		Logger::error("Invalid Shader Type");
	return 0;
}

OpenGLShaderProgram::~OpenGLShaderProgram() {
	glDeleteProgram(programID);
}