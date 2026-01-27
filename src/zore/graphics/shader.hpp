#pragma once
#include "zore/math/matrix/mat2.hpp"
#include "zore/math/matrix/mat3.hpp"
#include "zore/math/matrix/mat4.hpp"
#include <unordered_map>
#include <string>

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
		virtual ~Shader();

		static void SetShaderVersion(int major, int minor, bool core);
		static void SetShaderFolder(const std::string& path);

		uint32_t GetID() const;
		Shader& SetSource(const std::string& filename);
		Shader& SetDefines(const std::vector<DefineItem>& defines);
		Shader& Compile();
		void Bind() const;

		void SetBool(const std::string& name, bool data);
		void SetBool(const std::string& name, int32_t data);
		void SetInt(const std::string& name, int32_t data);
		void SetInt2(const std::string& name, const zm::ivec2& data);
		void SetInt3(const std::string& name, const zm::ivec3& data);
		void SetInt4(const std::string& name, const zm::ivec4& data);
		void SetUInt(const std::string& name, uint32_t data);
		void SetUInt2(const std::string& name, const zm::uvec2& data);
		void SetUInt3(const std::string& name, const zm::uvec3& data);
		void SetUInt4(const std::string& name, const zm::uvec4& data);
		void SetFloat(const std::string& name, float data);
		void SetFloat2(const std::string& name, const zm::vec2& data);
		void SetFloat3(const std::string& name, const zm::vec3& data);
		void SetFloat4(const std::string& name, const zm::vec4& data);
		void SetMat2(const std::string& name, const zm::mat2& data);
		void SetMat3(const std::string& name, const zm::mat3& data);
		void SetMat4(const std::string& name, const zm::mat4& data);
		void SetTextureSlot(const std::string& name, uint32_t slot);
		void SetUniformBufferIndex(const std::string& name, uint32_t index);

	protected:
		virtual Stage ValidateShaderStage(const std::string_view& name);
		Stage GetStage(const std::string_view& name);
		uint32_t CreateShaderStage(const std::string_view& content);
		uint32_t GetUniformLocation(const std::string& name);

	protected:
		uint32_t m_id;
		std::string m_filename;
		std::unordered_map<std::string, int32_t> m_uniforms;
		std::unordered_map<Stage, std::string> m_defines;
	};
}