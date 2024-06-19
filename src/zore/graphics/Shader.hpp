#pragma once
#include "zore/utils/DataTypes.hpp"
#include <unordered_map>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace zore {

	//========================================================================
	//	Shader Class
	//========================================================================

	class Shader {
	public:
		enum class Stage { VERTEX, GEOMETRY, FRAGMENT, COMPUTE, INVALID };

		struct DefineItem {
			Stage stage;
			std::string entry;
		};

	public:
		Shader();
		Shader(const std::string& filename, const std::vector<DefineItem>& defines = {});
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;
		~Shader();

		static void SetShaderVersion(int major, int minor, bool core);
		static void SetShaderFolder(const std::string& path);

		uint32_t GetID() const;
		Shader& SetSource(const std::string& filename);
		Shader& SetDefines(const std::vector<DefineItem>& defines);
		Shader& Compile();
		void Bind() const;

		void SetBool(const std::string& name, int32_t data);
		void SetInt(const std::string& name, int32_t data);
		void SetInt2(const std::string& name, const glm::ivec2& data);
		void SetInt3(const std::string& name, const glm::ivec3& data);
		void SetInt4(const std::string& name, const glm::ivec4& data);
		void SetUInt(const std::string& name, uint32_t data);
		void SetUInt2(const std::string& name, const glm::uvec2& data);
		void SetUInt3(const std::string& name, const glm::uvec3& data);
		void SetUInt4(const std::string& name, const glm::uvec4& data);
		void SetFloat(const std::string& name, float data);
		void SetFloat2(const std::string& name, const glm::vec2& data);
		void SetFloat3(const std::string& name, const glm::vec3& data);
		void SetFloat4(const std::string& name, const glm::vec4& data);
		void SetMat2(const std::string& name, const glm::mat2& data);
		void SetMat3(const std::string& name, const glm::mat3& data);
		void SetMat4(const std::string& name, const glm::mat4& data);
		void SetTextureSlot(const std::string& name, uint32_t slot);
		void SetUniformBufferIndex(const std::string& name, uint32_t index);

	private:
		struct StageData {
			StageData(const std::string& name, const std::string& source);
			std::string m_name;
			std::string m_source;
			uint32_t m_id;
			Stage m_type;
		};

	private:
		void CreateShaderStage(StageData& stage);
		void Link(std::vector<StageData>& stages);
		uint32_t GetUniformLocation(const std::string& name);

	private:
		uint32_t m_id;
		std::string m_filename;
		std::unordered_map<std::string, int32_t> m_uniforms;
		std::unordered_map<Stage, std::string> m_defines;
	};
}