#include "graphics/opengl/GLShader.hpp"
#include "utils/FileManager.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace zore {

	//========================================================================
	//	Shader Utilities
	//========================================================================

	unsigned int SDTtoGLDT(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Int:
			return GL_INT;
		case ShaderDataType::Float:
			return GL_FLOAT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		case ShaderDataType::Byte:
			return GL_BYTE;
		case ShaderDataType::UByte:
			return GL_UNSIGNED_BYTE;
		default:
			return GL_FALSE;
		}
	}

	unsigned int STRtoGLST(const std::string& stage) {
		if (stage == "vertex")
			return GL_VERTEX_SHADER;
		else if (stage == "fragment")
			return GL_FRAGMENT_SHADER;

		Logger::Error("Invalid Shader Stage requested");
		return 0;
	}

	std::string GLSTtoSTR(unsigned int stage) {
		if (stage == GL_VERTEX_SHADER)
			return "vertex";
		else if (stage == GL_FRAGMENT_SHADER)
			return "fragment";

		Logger::Error("Invalid Shader Stage requested");
		return "";
	}

	//========================================================================
	//	Shader Program Class
	//========================================================================

	GLShader::GLShader(const std::string& filename) {
		shaderID = glCreateProgram();

		std::vector<GLShaderStage> shaderStages;
		
		std::string path = FileManager::GetPath("assets/shaders/" + filename + ".glsl");
		std::ifstream f(path);
		ENSURE(f.is_open(), "Error opening file: " + path);

		// Read file
		std::string line;
		while (std::getline(f, line)) {
			if (line.find("#shaderstage") != std::string::npos)
				shaderStages.push_back({ line.substr(13, std::string::npos) });
			else if (line != "")
				shaderStages[shaderStages.size() - 1].content += line + "\n";
		}

		Link(shaderStages);
	}

	GLShader::~GLShader() {
		glDeleteProgram(shaderID);
	}

	void GLShader::Bind() {
		glUseProgram(shaderID);
	}

	void GLShader::UnBind() {
		glUseProgram(0);
	}

	unsigned int GLShader::GetShaderID() {
		return shaderID;
	}

	void GLShader::CreateShaderStage(GLShaderStage& stage) {
		stage.id = glCreateShader(stage.type);

		const char* source = stage.content.c_str();
		glShaderSource(stage.id, 1, &source, NULL);
		glCompileShader(stage.id);
		// Check if shader was created successfully
		GLint status;
		glGetShaderiv(stage.id, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[256];
			glGetShaderInfoLog(stage.id, 256, NULL, buffer);
			throw ZORE_EXCEPTION("Error compiling shader: " + name + "\n" + std::string(buffer));
		}
		glAttachShader(shaderID, stage.id);
	}

	void GLShader::Link(std::vector<GLShaderStage>& shaderStages) {
		for (GLShaderStage& stage : shaderStages)
			CreateShaderStage(stage);

		glLinkProgram(shaderID);
		// Check if shaders were linked successfully
		int status;
		glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
		if (!status) {
			char buffer[256];
			glGetProgramInfoLog(shaderID, 256, NULL, buffer);
			throw ZORE_EXCEPTION("Error linking shader:\n" + std::string(buffer));
		}

		for (GLShaderStage& stage : shaderStages) {
			glDetachShader(shaderID, stage.id);
			glDeleteShader(stage.id);
		}
	}

	unsigned int GLShader::GetUniformLoc(const std::string& name) {
		auto iter = uniforms.find(name);
		if (iter == uniforms.end()) {
			unsigned int loc = glGetUniformLocation(shaderID, name.c_str());
			if (loc == -1) Logger::Warn("Invalid uniform name: " + name);
			uniforms.insert({ name, loc });
			return loc;
		}
		return iter->second;
	}

	void GLShader::SetInt(const std::string& name, int data) {
		glUniform1i(GetUniformLoc(name), data);
	}

	void GLShader::SetInt2(const std::string& name, const glm::ivec2& data) {
		glUniform2i(GetUniformLoc(name), data.x, data.y);
	}

	void GLShader::SetInt3(const std::string& name, const glm::ivec3& data) {
		glUniform3i(GetUniformLoc(name), data.x, data.y, data.z);
	}

	void GLShader::SetInt4(const std::string& name, const glm::ivec4& data) {
		glUniform4i(GetUniformLoc(name), data.x, data.y, data.z, data.w);
	}

	void GLShader::SetFloat(const std::string& name, float data) {
		glUniform1f(GetUniformLoc(name), data);
	}

	void GLShader::SetFloat2(const std::string& name, const glm::vec2& data) {
		glUniform2f(GetUniformLoc(name), data.x, data.y);
	}

	void GLShader::SetFloat3(const std::string& name, const glm::vec3& data) {
		glUniform3f(GetUniformLoc(name), data.x, data.y, data.z);
	}

	void GLShader::SetFloat4(const std::string& name, const glm::vec4& data) {
		glUniform4f(GetUniformLoc(name), data.x, data.y, data.z, data.w);
	}

	void GLShader::SetMat4(const std::string& name, const glm::mat4& data) {
		glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, glm::value_ptr(data));
	}
}