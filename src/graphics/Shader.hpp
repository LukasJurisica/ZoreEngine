#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include <string>

namespace zore {

	//========================================================================
	//	Platform Agnostic Shader Interface
	//========================================================================

	enum class ShaderDataType { Int, Float, Mat, Bool, Byte, UByte };
	unsigned int ShaderDataTypeSize(ShaderDataType type, unsigned int count);

	class Shader {
	public:
		static Shader* Create(const std::string& name);
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		static void UnBind();
		
		virtual void SetInt(const std::string& name, int data) = 0;
		virtual void SetInt2(const std::string& name, const glm::ivec2& data) = 0;
		virtual void SetInt3(const std::string& name, const glm::ivec3& data) = 0;
		virtual void SetInt4(const std::string& name, const glm::ivec4& data) = 0;
		virtual void SetFloat(const std::string& name, float data) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& data) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& data) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& data) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& data) = 0;
	};
}