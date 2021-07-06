#pragma once
#include <vector>
#include <unordered_map>
#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Shader Utilities
	//========================================================================

	unsigned int SDTtoGLDT(ShaderDataType type);
	unsigned int STRtoGLST(const std::string& stage);
	std::string GLSTtoSTR(unsigned int);

	//========================================================================
	//	OpenGL Shader Class
	//========================================================================

	struct GLShaderStage {
		unsigned int type;
		unsigned int id;
		std::string content;

		GLShaderStage(std::string type) : type(STRtoGLST(type)), content(""), id(0) {}
	};

	class GLShader : public Shader {
	public:
		GLShader(const std::string& filename);
		~GLShader();

		void Bind() override;
		static void UnBind();
		unsigned int GetShaderID();

		void SetInt(const std::string& name, int data) override;
		void SetInt2(const std::string& name, const glm::ivec2& data) override;
		void SetInt3(const std::string& name, const glm::ivec3& data) override;
		void SetInt4(const std::string& name, const glm::ivec4& data) override;
		void SetFloat(const std::string& name, float data) override;
		void SetFloat2(const std::string& name, const glm::vec2& data) override;
		void SetFloat3(const std::string& name, const glm::vec3& data) override;
		void SetFloat4(const std::string& name, const glm::vec4& data) override;
		void SetMat4(const std::string& name, const glm::mat4& data) override;

	private:
		void CreateShaderStage(GLShaderStage& stage);
		void Link(std::vector<GLShaderStage>& shaderStages);
		unsigned int GetUniformLoc(const std::string& name);

		/*
		//std::unordered_map<std::string, unsigned int> attributes;
		*/
		std::unordered_map<std::string, int> uniforms;
		unsigned int shaderID;
		std::string name;
	};
}