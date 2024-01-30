#include "zore/graphics/Shader.hpp"
#include "zore/core/FileManager.hpp"
#include "zore/utils/StringUtils.hpp"
#include "zore/debug/Debug.hpp"
#include <glad/glad.h>

#define SHADER_ERROR_BUFFER_LENGTH 256

namespace zore {

	//========================================================================
	//	Shader Class
	//========================================================================

	const static Shader* s_active = nullptr;
	static std::string s_version = "";

	Shader::Shader() : m_id(0) {}

	Shader::Shader(const std::string& filename, const std::vector<DefineItem>& defines) : m_id(0), m_filename(filename) {
		SetDefines(defines);
		Compile();
	}

	Shader::~Shader() {
		glDeleteProgram(m_id);
	}

	void Shader::SetShaderVersion(const std::string& version) {
		s_version = version + "\n";
	}

	uint32_t Shader::GetID() const {
		return m_id;
	}

	Shader& Shader::SetSource(const std::string& filename) {
		m_filename = filename;
		return *this;
	}

	Shader& Shader::SetDefines(const std::vector<DefineItem>& defines) {
		const Stage validStages[] = { Stage::VERTEX, Stage::GEOMETRY, Stage::FRAGMENT, Stage::COMPUTE };
		for (const Stage& stage : validStages)
			m_defines[stage] = "";
		for (const DefineItem& item : defines)
			m_defines[item.stage] += "#define " + item.entry + "\n";
		return *this;
	}

	void Shader::Compile() {
		// Cleanup from previous compilation
		if (m_id) {
			glDeleteProgram(m_id);
			m_uniforms.clear();
			s_active = nullptr;
		}
		m_id = glCreateProgram();

		std::vector<StageData> stages;
		std::string source;
		FileManager::ReadContent(source, "assets/shaders/" + m_filename, true, IS_DEBUG);
		size_t left = 0;
		size_t right = std::string::npos;

		// Parse Shader file, and create shader stages
		do {
			left = source.rfind("#shaderstage", right);
			if (left == std::string::npos) // No shaderstage found
				break;

			size_t start = source.find("\n", left);
			CreateShaderStage(stages.emplace_back(StringUtils::SubStrRange(source, left + 13, start), StringUtils::SubStrRange(source, start + 1, right)));

			right = left - 1;
		} while (left > 0);

		Link(stages);
	}

	void Shader::Bind() const {
		if (s_active != this) {
			glUseProgram(m_id);
			s_active = this;
		}
	}

	Shader::StageData::StageData(const std::string& stage, const std::string& source) : m_name(stage), m_source(source), m_id(0) {
		if (stage == "vertex")
			m_type = Stage::VERTEX;
		else if (stage == "geometry")
			m_type = Stage::GEOMETRY;
		else if (stage == "fragment")
			m_type = Stage::FRAGMENT;
		else if (stage == "compute")
			m_type = Stage::COMPUTE;
		else
			m_type = Stage::INVALID;
	}

	void Shader::CreateShaderStage(StageData& stage) {
		ENSURE(stage.m_type != Stage::INVALID, "Invalid Shader Stage requested (" + stage.m_name + ") in file: " + m_filename);
		
		// Add defines and version to shader source
		stage.m_source = s_version + m_defines[stage.m_type] + stage.m_source;

		// Create and compile shader stages
		static const uint32_t GLStages[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER, GL_INVALID_ENUM };
		stage.m_id = glCreateShader(GLStages[static_cast<uint32_t>(stage.m_type)]);
		const char* source = stage.m_source.c_str();
		glShaderSource(stage.m_id, 1, &source, nullptr);
		glCompileShader(stage.m_id);

		// Check if shader stage was created successfully
		int32_t status;
		glGetShaderiv(stage.m_id, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[SHADER_ERROR_BUFFER_LENGTH];
			glGetShaderInfoLog(stage.m_id, SHADER_ERROR_BUFFER_LENGTH, nullptr, buffer);
			throw ZORE_EXCEPTION("Error compiling " + stage.m_name + " shader: " + m_filename + "\n" + std::string(buffer));
		}
	}

	void Shader::Link(std::vector<StageData>& stages) {
		for (StageData& stage : stages)
			glAttachShader(m_id, stage.m_id);

		glLinkProgram(m_id);
		// Check if shaders were linked successfully
		int32_t status;
		glGetProgramiv(m_id, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[SHADER_ERROR_BUFFER_LENGTH];
			glGetProgramInfoLog(m_id, SHADER_ERROR_BUFFER_LENGTH, nullptr, buffer);
			throw ZORE_EXCEPTION("Error linking shader: " + m_filename + "\n" + std::string(buffer));
		}

		// Cleanup resources from shader stages
		for (StageData& stage : stages) {
			glDetachShader(m_id, stage.m_id);
			glDeleteShader(stage.m_id);
		}
	}

	uint32_t Shader::GetUniformLocation(const std::string& name) {
		Bind();
		auto iter = m_uniforms.find(name);
		if (iter == m_uniforms.end()) {
			uint32_t loc = glGetUniformLocation(m_id, name.c_str());
			if (loc == -1)
				Logger::Warn("Invalid uniform name: " + name);
			m_uniforms.insert({ name, loc });
			return loc;
		}
		return iter->second;
	}

	// glUniform{1|2|3|4}{f|i|ui}
	void Shader::SetBool(const std::string& name, int32_t data) {
		glUniform1i(GetUniformLocation(name), data);
	}

	void Shader::SetInt(const std::string& name, int32_t data) {
		glUniform1i(GetUniformLocation(name), data);
	}

	void Shader::SetInt2(const std::string& name, const glm::ivec2& data) {
		glUniform2i(GetUniformLocation(name), data.x, data.y);
	}

	void Shader::SetInt3(const std::string& name, const glm::ivec3& data) {
		glUniform3i(GetUniformLocation(name), data.x, data.y, data.z);
	}

	void Shader::SetInt4(const std::string& name, const glm::ivec4& data) {
		glUniform4i(GetUniformLocation(name), data.x, data.y, data.z, data.w);
	}

	void Shader::SetUInt(const std::string& name, uint32_t data) {
		glUniform1ui(GetUniformLocation(name), data);
	}

	void Shader::SetUInt2(const std::string& name, const glm::uvec2& data) {
		glUniform2ui(GetUniformLocation(name), data.x, data.y);
	}

	void Shader::SetUInt3(const std::string& name, const glm::uvec3& data) {
		glUniform3ui(GetUniformLocation(name), data.x, data.y, data.z);
	}

	void Shader::SetUInt4(const std::string& name, const glm::uvec4& data) {
		glUniform4ui(GetUniformLocation(name), data.x, data.y, data.z, data.w);
	}

	void Shader::SetFloat(const std::string& name, float data) {
		glUniform1f(GetUniformLocation(name), data);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& data) {
		glUniform2f(GetUniformLocation(name), data.x, data.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& data) {
		glUniform3f(GetUniformLocation(name), data.x, data.y, data.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& data) {
		glUniform4f(GetUniformLocation(name), data.x, data.y, data.z, data.w);
	}
	// glUniformMatrix{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3}fv
	void Shader::SetMat3(const std::string& name, const glm::mat3& data) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &(data[0].x));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& data) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &(data[0].x));
	}

	void Shader::SetTextureSlot(const std::string& name, uint32_t slot) {
		glUniform1i(GetUniformLocation(name), slot);
	}

	void Shader::SetUniformBufferIndex(const std::string& name, uint32_t index) {
		glUniformBlockBinding(m_id, glGetUniformBlockIndex(m_id, name.c_str()), index);
	}
}