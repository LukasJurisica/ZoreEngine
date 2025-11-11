#pragma once

#include "zore/graphics/graphics_core.hpp"
#include <utility>
#include <vector>
#include <string>

namespace zore {

	//========================================================================
	//	Shader
	//========================================================================

	class Shader {
	public:
		enum class Stage { VERTEX, GEOMETRY, FRAGMENT, INVALID };
		struct Define { Stage stage; std::string entry; };

	public:
		Shader() = default;
		Shader(const std::string& filename, const std::vector<Define>& defines);
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;
		~Shader();

		static void SetShaderVersion(int major, int minor, bool core);
		static void SetShaderFolder(const std::string& path);

		Shader& SetSource(const std::string& filename);
		Shader& ClearDefines();
		Shader& AddDefines(const std::vector<Define>& defines);
		Shader& AddDefine(const Define& define);
		const std::vector<std::pair<Stage, handle_t>>& GetStages() const;
		void Compile();

	private:
		Stage GetStage(const std::string_view& name);
		void CreateShaderStage(const std::string_view& content);
		Stage ValidateShaderStage(const std::string_view& name);

	protected:
		std::string m_filename;
		std::vector<std::pair<Stage, handle_t>> m_shader_stages;
		static inline std::string s_shader_folder = "assets/shaders/";
	};
}