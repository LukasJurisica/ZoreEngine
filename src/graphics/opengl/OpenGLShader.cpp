#include "graphics/opengl/OpenGLShader.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "utils/Logger.hpp"
#include "utils/FileManager.hpp"

static GLenum shaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	else if (type == "fragment")
		return GL_FRAGMENT_SHADER;
	else
		Logger::error("Invalid Shader Type");
	return 0;
}

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& filename) {
	programID = glCreateProgram();
	const std::string& shaderSource = FileManager::readTextFile(filename);
	std::vector<GLuint>& shaders = process(shaderSource);

	if (shaders.size() != 2)
		Logger::error("Must have exactly 2 (a vertex and a fragment) shader");

	setupProgram();
	for (GLuint shader : shaders) {
		glDetachShader(programID, shader);
		glDeleteShader(shader);
	}
}

std::vector<GLuint> OpenGLShaderProgram::process(const std::string& source) {
	std::vector<GLuint> shaders;

	std::string token = "#type";
	size_t tokenLength = token.length();
	size_t pos = source.find(token, 0);
	// Check if there are any more shaders in the file
	while (pos != std::string::npos) {
		size_t end = source.find_first_of("\r\n", pos);
		size_t start = pos + tokenLength + 1;
		GLenum type = shaderTypeFromString(source.substr(start, end - start));
		size_t next = source.find_first_not_of("\r\n", end);
		pos = source.find(token, next);

		GLuint shader = glCreateShader(type);
		setupShader((pos == std::string::npos) ? source.substr(next) : source.substr(next, pos - next), shader);
		shaders.push_back(shader);
	}
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
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(value));
}

OpenGLShaderProgram::~OpenGLShaderProgram() {
	glDeleteProgram(programID);
}