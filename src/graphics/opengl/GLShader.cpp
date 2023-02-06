#include "graphics/opengl/GLShader.hpp"
#include "graphics/Texture.hpp"
#include "utils/FileManager.hpp"
#include "utils/StringUtils.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

#define SHADER_ERROR_BUFFER_LENGTH 256

namespace zore {

	//========================================================================
	//	OpenGL Shader Utilities
	//========================================================================

	GLShaderStage::GLShaderStage(const std::string& name) : name(name) {
		type = static_cast<uint>(Shader::GetStage(name));
	}

	static const GLShader* boundShader = nullptr;

	//========================================================================
	//	Shader Program Class
	//========================================================================

	GLShader::GLShader(const std::string& filename, const std::vector<ShaderDefineItem>& defines) : name(filename + ".glsl") {
		id = glCreateProgram();

		// Create define list strings
		std::string defineContent[3] = { "", "", "" };
		for (const ShaderDefineItem& item : defines)
			if (item.entry != "")
				defineContent[static_cast<uint>(item.stage)] += "#define " + item.entry + "\n";

		// Initialize variables for parsing shader file
		std::vector<uint> shaderStages;
		std::string source;
		FileManager::ReadContent(source, "assets/shaders/" + name, true, IS_DEBUG);
		size_t left = 0;
		size_t right = std::string::npos;

		// Parse Shader file, and create shader stages
		do {
			left = source.rfind("#shaderstage", right);
			if (left == std::string::npos) // No shaderstage found
				break;

			size_t start = source.find("\n", left);
			GLShaderStage stage(StringUtils::SubStrRange(source, left + 13, start));
			stage.source = StringUtils::SubStrRange(source, start + 1, right);

			shaderStages.push_back(CreateShaderStage(stage, defineContent[stage.type]));
			right = left - 1;

		} while (left > 0);

		Link(shaderStages);
	}

	GLShader::~GLShader() {
		glDeleteProgram(id);
	}

	uint GLShader::GetShaderID() {
		return id;
	}

	void GLShader::Bind() const {
		if (boundShader != this) {
			glUseProgram(id);
			boundShader = this;
		}
	}

	void GLShader::Unbind() const {
		glUseProgram(0);
	}

	unsigned int GLShader::CreateShaderStage(GLShaderStage& stage, const std::string& defines) {
		// Add defines to shader source
		size_t index = stage.source.find("#version");
		if (index == std::string::npos)
			stage.source = defines + stage.source;
		else {
			index = stage.source.find("\n", index);
			stage.source.insert(index + 1, defines);
		}

		static const uint GLStages[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
		uint stageID = glCreateShader(GLStages[stage.type]);
		const char* source = stage.source.c_str();
		glShaderSource(stageID, 1, &source, nullptr);
		glCompileShader(stageID);

		// Check if shader stage was created successfully
		GLint status;
		glGetShaderiv(stageID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[SHADER_ERROR_BUFFER_LENGTH];
			glGetShaderInfoLog(stageID, SHADER_ERROR_BUFFER_LENGTH, nullptr, buffer);
			throw ZORE_EXCEPTION("Error compiling " + stage.name + " shader: " + name + "\n" + std::string(buffer));
		}

		return stageID;
	}

	void GLShader::Link(std::vector<unsigned int>& shaderStages) {
		for (uint stageID : shaderStages)
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

		for (uint stageID : shaderStages) {
			glDetachShader(id, stageID);
			glDeleteShader(stageID);
		}
	}

	unsigned int GLShader::GetUniformLoc(const std::string& name) {
		auto iter = uniforms.find(name);
		if (iter == uniforms.end()) {
			uint loc = glGetUniformLocation(id, name.c_str());
			if (loc == -1)
				Logger::Warn("Invalid uniform name: " + name);
			uniforms.insert({ name, loc });
			return loc;
		}
		return iter->second;
	}
	// glUniform{1|2|3|4}{f|i|ui}
	void GLShader::SetBool(const std::string& name, int data) {
		Bind();
		glUniform1i(GetUniformLoc(name), data);
	}

	void GLShader::SetInt(const std::string& name, int data) {
		Bind();
		glUniform1i(GetUniformLoc(name), data);
	}

	void GLShader::SetInt2(const std::string& name, const glm::ivec2& data) {
		Bind();
		glUniform2i(GetUniformLoc(name), data.x, data.y);
	}

	void GLShader::SetInt3(const std::string& name, const glm::ivec3& data) {
		Bind();
		glUniform3i(GetUniformLoc(name), data.x, data.y, data.z);
	}

	void GLShader::SetInt4(const std::string& name, const glm::ivec4& data) {
		Bind();
		glUniform4i(GetUniformLoc(name), data.x, data.y, data.z, data.w);
	}

	void GLShader::SetUInt(const std::string& name, uint data) {
		Bind();
		glUniform1ui(GetUniformLoc(name), data);
	}

	void GLShader::SetUInt2(const std::string& name, const glm::uvec2& data) {
		Bind();
		glUniform2ui(GetUniformLoc(name), data.x, data.y);
	}

	void GLShader::SetUInt3(const std::string& name, const glm::uvec3& data) {
		Bind();
		glUniform3ui(GetUniformLoc(name), data.x, data.y, data.z);
	}

	void GLShader::SetUInt4(const std::string& name, const glm::uvec4& data) {
		Bind();
		glUniform4ui(GetUniformLoc(name), data.x, data.y, data.z, data.w);
	}

	void GLShader::SetFloat(const std::string& name, float data) {
		Bind();
		glUniform1f(GetUniformLoc(name), data);
	}

	void GLShader::SetFloat2(const std::string& name, const glm::vec2& data) {
		Bind();
		glUniform2f(GetUniformLoc(name), data.x, data.y);
	}

	void GLShader::SetFloat3(const std::string& name, const glm::vec3& data) {
		Bind();
		glUniform3f(GetUniformLoc(name), data.x, data.y, data.z);
	}

	void GLShader::SetFloat4(const std::string& name, const glm::vec4& data) {
		Bind();
		glUniform4f(GetUniformLoc(name), data.x, data.y, data.z, data.w);
	}
	// glUniformMatrix{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3}fv
	void GLShader::SetMat3(const std::string& name, const glm::mat3& data) {
		Bind();
		glUniformMatrix3fv(GetUniformLoc(name), 1, GL_FALSE, &(data[0].x));
	}

	void GLShader::SetMat4(const std::string& name, const glm::mat4& data) {
		Bind();
		glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &(data[0].x));
	}

	void GLShader::SetTextureSlot(const std::string& name, uint slot) {
		Bind();
		glUniform1i(GetUniformLoc(name), slot);
	}

	void GLShader::SetUniformBufferIndex(const std::string& name, uint index) {
		Bind();
		glUniformBlockBinding(id, glGetUniformBlockIndex(id, name.c_str()), index);
	}
}