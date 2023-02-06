#pragma once
#include <vector>
#include <unordered_map>
#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Shader Utilities
	//========================================================================
	
	struct GLShaderStage {
		GLShaderStage(const std::string& name);
		uint type;
		std::string name;
		std::string source;
	};

	//========================================================================
	//	OpenGL Shader Class
	//========================================================================

	class GLShader : public Shader {
	public:
		GLShader(const std::string& filename, const std::vector<ShaderDefineItem>& defines);
		~GLShader();

		uint GetShaderID();
		void Bind() const override;
		void Unbind() const override;

		void SetBool(const std::string& name, int data) override;
		void SetInt(const std::string& name, int data) override;
		void SetInt2(const std::string& name, const glm::ivec2& data) override;
		void SetInt3(const std::string& name, const glm::ivec3& data) override;
		void SetInt4(const std::string& name, const glm::ivec4& data) override;
		void SetUInt(const std::string& name, uint data) override;
		void SetUInt2(const std::string& name, const glm::uvec2& data) override;
		void SetUInt3(const std::string& name, const glm::uvec3& data) override;
		void SetUInt4(const std::string& name, const glm::uvec4& data) override;
		void SetFloat(const std::string& name, float data) override;
		void SetFloat2(const std::string& name, const glm::vec2& data) override;
		void SetFloat3(const std::string& name, const glm::vec3& data) override;
		void SetFloat4(const std::string& name, const glm::vec4& data) override;
		void SetMat3(const std::string& name, const glm::mat3& data) override;
		void SetMat4(const std::string& name, const glm::mat4& data) override;
		void SetTextureSlot(const std::string& name, uint slot) override;
		void SetUniformBufferIndex(const std::string& name, uint index) override;

	private:
		unsigned int CreateShaderStage(GLShaderStage& stage, const std::string& defines);
		void Link(std::vector<uint>& shaderStages);
		unsigned int GetUniformLoc(const std::string& name);

		std::unordered_map<std::string, int> uniforms;
		uint id;
		std::string name;
	};
}