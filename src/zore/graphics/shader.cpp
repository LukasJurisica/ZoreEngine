#include "zore/graphics/shader.hpp"
#include "zore/core/file_manager.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

#define SHADER_ERROR_BUFFER_LENGTH 256

namespace zore {

	//========================================================================
	//	Shader Class
	//========================================================================

	const static Shader* s_active = nullptr;
	static std::string s_version = "";
	static std::string s_shader_folder = "assets/shaders/";

	Shader::Shader() : m_id(0) {}

	Shader::Shader(const std::string& filename, const std::vector<DefineItem>& defines) : m_id(0), m_filename(filename) {
		SetDefines(defines);
		Compile();
	}

	Shader::~Shader() {
		glDeleteProgram(m_id);
	}

	void Shader::SetShaderVersion(int major, int minor, bool core) {
		int version = ((major * 10) + minor) * 10;
		s_version = "#version " + TOSTR(version) + (core ? " core\n" : " compatibility\n");
	}

	void Shader::SetShaderFolder(const std::string& path) {
		s_shader_folder = path;
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

	Shader& Shader::Compile() {
		// Cleanup from previous compilation
		if (m_id) {
			glDeleteProgram(m_id);
			m_uniforms.clear();
			s_active = nullptr;
		}
		m_id = glCreateProgram();

		std::string source;
		FileManager::ReadContent(source, s_shader_folder + m_filename, IS_DEBUG);
		size_t right = source.size();
		size_t left = source.rfind("#shaderstage", right);

		// Parse Shader file, and create shader stages
		std::vector<uint32_t> stages;
		while (left != std::string::npos) {
			stages.push_back(CreateShaderStage(std::string_view(source.data() + left, right - left)));
			source[left] = '\0';
			right = left - 1;
			left = source.rfind("#shaderstage", right);
		}

		for (uint32_t stage_id : stages)
			glAttachShader(m_id, stage_id);

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
		for (uint32_t stage_id : stages) {
			glDetachShader(m_id, stage_id);
			glDeleteShader(stage_id);
		}
		stages.clear();
		return *this;
	}

	void Shader::Bind() const {
		if (s_active != this) {
			glUseProgram(m_id);
			s_active = this;
		}
	}

	// glUniform{1|2|3|4}{f|i|ui}v
	void Shader::SetBool(const std::string& name, bool data) {
		glUniform1i(GetUniformLocation(name), static_cast<int32_t>(data));
	}

	void Shader::SetBool(const std::string& name, int32_t data) {
		glUniform1i(GetUniformLocation(name), data);
	}

	void Shader::SetInt(const std::string& name, int32_t data) {
		glUniform1i(GetUniformLocation(name), data);
	}

	void Shader::SetInt2(const std::string& name, const zm::ivec2& data) {
		glUniform2iv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetInt3(const std::string& name, const zm::ivec3& data) {
		glUniform3iv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetInt4(const std::string& name, const zm::ivec4& data) {
		glUniform4iv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetUInt(const std::string& name, uint32_t data) {
		glUniform1ui(GetUniformLocation(name), data);
	}

	void Shader::SetUInt2(const std::string& name, const zm::uvec2& data) {
		glUniform2uiv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetUInt3(const std::string& name, const zm::uvec3& data) {
		glUniform3uiv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetUInt4(const std::string& name, const zm::uvec4& data) {
		glUniform4uiv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetFloat(const std::string& name, float data) {
		glUniform1f(GetUniformLocation(name), data);
	}

	void Shader::SetFloat2(const std::string& name, const zm::vec2& data) {
		glUniform2fv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetFloat3(const std::string& name, const zm::vec3& data) {
		glUniform3fv(GetUniformLocation(name), 1, data.data);
	}

	void Shader::SetFloat4(const std::string& name, const zm::vec4& data) {
		glUniform4fv(GetUniformLocation(name), 1, data.data);
	}
	// glUniformMatrix{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3}fv
	void Shader::SetMat2(const std::string& name, const zm::mat2& data) {
		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_TRUE, &(data[0].x));
	}

	void Shader::SetMat3(const std::string& name, const zm::mat3& data) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_TRUE, &(data[0].x));
	}

	void Shader::SetMat4(const std::string& name, const zm::mat4& data) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, &(data[0].x));
	}

	void Shader::SetTextureSlot(const std::string& name, uint32_t slot) {
		glUniform1i(GetUniformLocation(name), slot);
	}

	void Shader::SetUniformBufferIndex(const std::string& name, uint32_t index) {
		glUniformBlockBinding(m_id, glGetUniformBlockIndex(m_id, name.c_str()), index);
	}
	
	Shader::Stage Shader::ValidateShaderStage(const std::string_view& name) {
		Stage stage = GetStage(name);
		ENSURE(stage != Stage::INVALID, std::format("Invalid ({}) Shader Stage requested in file: {}", name, m_filename));
		ENSURE(stage != Stage::COMPUTE, std::format("Invalid ({}) Shader stage requested in render shader: {}", name, m_filename));
		return stage;
	}

	Shader::Stage Shader::GetStage(const std::string_view& name) {
		if (name == "vertex")
			return Stage::VERTEX;
		else if (name == "geometry")
			return Stage::GEOMETRY;
		else if (name == "fragment")
			return Stage::FRAGMENT;
		else if (name == "compute")
			return Stage::COMPUTE;
		else
			return Stage::INVALID;
	}

	uint32_t Shader::CreateShaderStage(const std::string_view& content) {
		size_t start = content.find("\n", 0);
		std::string_view stage_name = std::string_view(content.data() + 13, start - 13);
		Stage stage = ValidateShaderStage(stage_name);
		std::string source = s_version + m_defines[stage] + std::string(content.data() + start + 1, content.size() - start - 1);

		static const uint32_t GLStages[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER, GL_INVALID_ENUM };
		uint32_t stage_id = glCreateShader(GLStages[static_cast<uint32_t>(stage)]);
		const char* source_c = source.c_str();
		glShaderSource(stage_id, 1, &source_c, nullptr);
		glCompileShader(stage_id);

		// Check if shader stage was created successfully
		int32_t status;
		glGetShaderiv(stage_id, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[SHADER_ERROR_BUFFER_LENGTH];
			glGetShaderInfoLog(stage_id, SHADER_ERROR_BUFFER_LENGTH, nullptr, buffer);
			throw ZORE_EXCEPTION(std::format("Error compiling {} shader: {}\n{}", stage_name, m_filename, buffer));
		}
		return stage_id;
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
}