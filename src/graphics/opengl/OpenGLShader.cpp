#include "graphics/opengl/OpenGLShader.hpp"
#include <glad/glad.h>
#include "core/main.hpp"

#define SHADER_DIRECTORY BASE_DIRECTORY "/src/graphics/opengl/shaders/"

OpenGLShader::OpenGLShader(const char* vertPath, const char* fragPath) {
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

void OpenGLShader::setupShader(unsigned int shader, const char* filename) {
	std::string source = FileManager::readTextFile(SHADER_DIRECTORY + std::string(filename));
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

void OpenGLShader::setupProgram() {
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

void OpenGLShader::bind() {
	glUseProgram(programID);
}

void OpenGLShader::setUniformInt(const char* name, int value) {

}

void OpenGLShader::setUniformFloat(const char* name, float value) {

}

void OpenGLShader::setUniformFloat2(const char* name, const glm::vec2& value) {

}

void OpenGLShader::setUniformFloat3(const char* name, const glm::vec3& value) {

}

void OpenGLShader::setUniformFloat4(const char* name, const glm::vec4& value) {

}

void OpenGLShader::setUniformMat4(const char* name, const glm::mat4& value) {

}

OpenGLShader::~OpenGLShader() {
	glDeleteProgram(programID);
}