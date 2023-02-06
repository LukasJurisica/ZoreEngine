#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "utils/DataTypes.hpp"
#include <string>
#include <vector>

namespace zore {

	//========================================================================
	//	Platform Agnostic Shader Interface
	//========================================================================

	enum class ShaderStage { VERTEX, GEOMETRY, FRAGMENT, INVALID };

	struct ShaderDefineItem {
		ShaderStage stage;
		std::string entry;
	};

	class Shader {
	public:
		static Shader* Create(const std::string& name, const std::vector<ShaderDefineItem>& defines = {});
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		static ShaderStage GetStage(const std::string& stage);
		
		virtual void SetBool(const std::string& name, int data) = 0;
		virtual void SetInt(const std::string& name, int data) = 0;
		virtual void SetInt2(const std::string& name, const glm::ivec2& data) = 0;
		virtual void SetInt3(const std::string& name, const glm::ivec3& data) = 0;
		virtual void SetInt4(const std::string& name, const glm::ivec4& data) = 0;
		virtual void SetUInt(const std::string& name, uint data) = 0;
		virtual void SetUInt2(const std::string& name, const glm::uvec2& data) = 0;
		virtual void SetUInt3(const std::string& name, const glm::uvec3& data) = 0;
		virtual void SetUInt4(const std::string& name, const glm::uvec4& data) = 0;
		virtual void SetFloat(const std::string& name, float data) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& data) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& data) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& data) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& data) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& data) = 0;
		virtual void SetTextureSlot(const std::string& name, uint slot) = 0;
		virtual void SetUniformBufferIndex(const std::string& name, uint index) = 0;
	};
}