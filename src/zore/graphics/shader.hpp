#pragma once
#include "zore/math/matrix/mat2.hpp"
#include "zore/math/matrix/mat3.hpp"
#include "zore/math/matrix/mat4.hpp"
#include "zore/io/asset_pack.hpp"
#include "zore/structures/string_unordered_map.hpp"
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
		Shader(std::string_view filename, const std::vector<DefineItem>& defines = {});
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;
		virtual ~Shader();

		static void SetShaderVersion(int major, int minor, bool core);
		static void SetShaderFolder(std::string_view path);

		uint32_t GetID() const;
		Shader& SetSource(std::string_view filename);
		Shader& SetSource(std::string_view filename, const AssetPack& asset_pack);
		Shader& SetDefines(const std::vector<DefineItem>& defines);
		Shader& Compile();
		void Bind() const;

		void SetBool(std::string_view name, bool data);
		void SetBool(std::string_view name, int32_t data);
		void SetInt(std::string_view name, int32_t data);
		void SetInt2(std::string_view name, const zm::ivec2& data);
		void SetInt3(std::string_view name, const zm::ivec3& data);
		void SetInt4(std::string_view name, const zm::ivec4& data);
		void SetUInt(std::string_view name, uint32_t data);
		void SetUInt2(std::string_view name, const zm::uvec2& data);
		void SetUInt3(std::string_view name, const zm::uvec3& data);
		void SetUInt4(std::string_view name, const zm::uvec4& data);
		void SetFloat(std::string_view name, float data);
		void SetFloat2(std::string_view name, const zm::vec2& data);
		void SetFloat3(std::string_view name, const zm::vec3& data);
		void SetFloat4(std::string_view name, const zm::vec4& data);
		void SetMat2(std::string_view name, const zm::mat2& data);
		void SetMat3(std::string_view name, const zm::mat3& data);
		void SetMat4(std::string_view name, const zm::mat4& data);
		void SetTextureSlot(std::string_view name, uint32_t slot);
		void SetUniformBufferIndex(std::string_view name, uint32_t index);

	protected:
		virtual Stage ValidateShaderStage(std::string_view name);
		Stage GetStage(std::string_view name);
		uint32_t CreateShaderStage(std::string_view content);
		uint32_t GetUniformLocation(std::string_view name);

	protected:
		uint32_t m_id;
		std::string m_filename;
		const AssetPack* m_asset_pack = nullptr;
		zore::string_unordered_map<int32_t> m_uniforms;
		std::unordered_map<Stage, std::string> m_defines;
	};
}