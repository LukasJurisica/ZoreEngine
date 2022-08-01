#include "graphics/opengl/GLShader.hpp"
#include "utils/FileManager.hpp"
#include "utils/StringUtils.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

#define SHADER_ERROR_BUFFER_LENGTH 256

namespace zore {

	//========================================================================
	//	Shader Utilities
	//========================================================================

	unsigned int STRtoGLST(const std::string& stage) {
		if (stage == "vertex")
			return GL_VERTEX_SHADER;
		else if (stage == "fragment")
			return GL_FRAGMENT_SHADER;
		else if (stage == "geometry")
			return GL_GEOMETRY_SHADER;

		Logger::Error("Invalid Shader Stage requested");
		return 0;
	}

	//========================================================================
	//	Shader Program Class
	//========================================================================

	GLShader::GLShader(const std::string& filename) : name(filename + ".glsl") {
		id = glCreateProgram();

		std::vector<unsigned int> shaderStages;
		std::string source;
		FileManager::ReadContent(source, "assets/shaders/" + name, true, false);

		size_t index = 0;
		do {
			index = source.rfind("#shaderstage", std::string::npos);
			shaderStages.push_back(CreateShaderStage(source.substr(index + 13, std::string::npos)));
			source.erase(index, std::string::npos);
		} while (index > 0);

		Link(shaderStages);
	}

	GLShader::~GLShader() {
		glDeleteProgram(id);
	}

	unsigned int GLShader::GetShaderID() {
		return id;
	}

	void GLShader::Bind() const {
		glUseProgram(id);
	}

	void GLShader::Unbind() const {
		glUseProgram(0);
	}

	unsigned int GLShader::CreateShaderStage(std::string& s) {
		size_t index = s.find("\n");
		std::string type = s.substr(0, index);
		StringUtils::TrimInPlace(type);
		unsigned int stageID = glCreateShader(STRtoGLST(type));

		const char* source = s.erase(0, index + 1).c_str();
		glShaderSource(stageID, 1, &source, nullptr);
		glCompileShader(stageID);

		// Check if shader stage was created successfully
		GLint status;
		glGetShaderiv(stageID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[SHADER_ERROR_BUFFER_LENGTH];
			glGetShaderInfoLog(stageID, SHADER_ERROR_BUFFER_LENGTH, nullptr, buffer);
			throw ZORE_EXCEPTION("Error compiling " + type + " shader: " + name + "\n" + std::string(buffer));
		}

		return stageID;
	}

	void GLShader::Link(std::vector<unsigned int>& shaderStages) {
		for (unsigned int stageID : shaderStages)
			glAttachShader(id, stageID);

		glLinkProgram(id);
		// Check if shaders were linked successfully
		int status;
		glGetProgramiv(id, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[SHADER_ERROR_BUFFER_LENGTH];
			glGetProgramInfoLog(id, SHADER_ERROR_BUFFER_LENGTH, nullptr, buffer);
			throw ZORE_EXCEPTION("Error linking shader: " + name + "\n" + std::string(buffer));
		}

		for (unsigned int stageID : shaderStages) {
			glDetachShader(id, stageID);
			glDeleteShader(stageID);
		}
	}

	unsigned int GLShader::GetUniformLoc(const std::string& name) {
		auto iter = uniforms.find(name);
		if (iter == uniforms.end()) {
			unsigned int loc = glGetUniformLocation(id, name.c_str());
			if (loc == -1)
				Logger::Warn("Invalid uniform name: " + name);
			uniforms.insert({ name, loc });
			return loc;
		}
		return iter->second;
	}
	// glUniform{1|2|3|4}{f|i|ui}
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

	void GLShader::SetUInt(const std::string& name, unsigned int data) {
		glUniform1ui(GetUniformLoc(name), data);
	}

	void GLShader::SetUInt2(const std::string& name, const glm::uvec2& data) {
		glUniform2ui(GetUniformLoc(name), data.x, data.y);
	}

	void GLShader::SetUInt3(const std::string& name, const glm::uvec3& data) {
		glUniform3ui(GetUniformLoc(name), data.x, data.y, data.z);
	}

	void GLShader::SetUInt4(const std::string& name, const glm::uvec4& data) {
		glUniform4ui(GetUniformLoc(name), data.x, data.y, data.z, data.w);
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
	// glUniformMatrix{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3}fv
	void GLShader::SetMat3(const std::string& name, const glm::mat3& data) {
		glUniformMatrix3fv(GetUniformLoc(name), 1, GL_FALSE, &(data[0].x));
	}

	void GLShader::SetMat4(const std::string& name, const glm::mat4& data) {
		glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &(data[0].x));
	}

	void GLShader::SetTextureSlot(const std::string& name, unsigned int index) {
		glUniform1i(GetUniformLoc(name), index);
	}

	void GLShader::SetUniformBufferIndex(const std::string& name, unsigned int index) {
		glUniformBlockBinding(id, glGetUniformBlockIndex(id, name.c_str()), index);
	}
}