#pragma once
#include <vector>
#include <unordered_map>
#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Shader Utilities
	//========================================================================

	// Generic Shader String name to OpenGL Shader Type
	unsigned int STRtoGLST(const std::string& stage);

	//========================================================================
	//	OpenGL Shader Class
	//========================================================================

	class GLShader : public Shader {
	public:
		GLShader(const std::string& filename);
		~GLShader();

		unsigned int GetShaderID();
		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int data) override;
		void SetInt2(const std::string& name, const glm::ivec2& data) override;
		void SetInt3(const std::string& name, const glm::ivec3& data) override;
		void SetInt4(const std::string& name, const glm::ivec4& data) override;
		void SetUInt(const std::string& name, unsigned int data) override;
		void SetUInt2(const std::string& name, const glm::uvec2& data) override;
		void SetUInt3(const std::string& name, const glm::uvec3& data) override;
		void SetUInt4(const std::string& name, const glm::uvec4& data) override;
		void SetFloat(const std::string& name, float data) override;
		void SetFloat2(const std::string& name, const glm::vec2& data) override;
		void SetFloat3(const std::string& name, const glm::vec3& data) override;
		void SetFloat4(const std::string& name, const glm::vec4& data) override;
		void SetMat3(const std::string& name, const glm::mat3& data) override;
		void SetMat4(const std::string& name, const glm::mat4& data) override;
		void SetTextureIndex(const std::string& name, unsigned int index) override;
		void SetUniformBufferIndex(const std::string& name, unsigned int index) override;

	private:
		unsigned int CreateShaderStage(std::string& source);
		void Link(std::vector<unsigned int>& shaderStages);
		unsigned int GetUniformLoc(const std::string& name);

		std::unordered_map<std::string, int> uniforms;
		unsigned int id;
		std::string name;
	};
}