#include "graphics/opengl/OpenGLShader.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "../../bin/config/config.h"
#include "utils/Logger.hpp"
#include "utils/FileManager.hpp"

#define GL_SHADER_DIRECTORY BASE_DIRECTORY "/src/graphics/opengl/shaders/"

OpenGLShaderProgram::OpenGLShaderProgram(const char* vertPath, const char* fragPath) {
	// Create a shader program
	programID = glCreateProgram();

	// Create Vertex and Fragment shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	setupShader(vs, vertPath);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	setupShader(fs, fragPath);

	// link the vertex and fragment shaders to the program
	setupProgram();

	// Delete memory used by shaders, since they were already compiled and linked
	glDetachShader(programID, vs);
	glDeleteShader(vs);
	glDetachShader(programID, fs);
	glDeleteShader(fs);
}

void OpenGLShaderProgram::setupShader(unsigned int shader, const char* filename) {
	std::string source = FileManager::readTextFile(GL_SHADER_DIRECTORY + std::string(filename));
	const char* source_p = source.c_str();

	glShaderSource(shader, 1, &source_p, NULL);
	glCompileShader(shader);
	// Check if shaders compiled successfully
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
		char* buffer = new char[status];
		glGetShaderInfoLog(shader, status, NULL, buffer);
		glDeleteShader(shader);
		Logger::error("Compiling (" + std::string(filename) + ") failed: " + std::string(buffer));
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