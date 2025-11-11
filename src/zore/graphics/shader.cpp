#include "zore/graphics/shader.hpp"
#include "zore/debug.hpp"

namespace zore {

	//========================================================================
	//	Shader Common Functions
	//========================================================================

	void Shader::SetShaderFolder(const std::string& path) {
		s_shader_folder = path;
	}

	const std::vector<std::pair<Shader::Stage, handle_t>>& Shader::GetStages() const {
		return m_shader_stages;
	}

	Shader& Shader::SetSource(const std::string& filename) {
		m_filename = filename;
		return *this;
	}

	//Shader& Shader::SetDefines(const std::vector<DefineItem>& defines) {
	//	const Stage valid_stages[] = { Stage::VERTEX, Stage::GEOMETRY, Stage::FRAGMENT, Stage::COMPUTE };
	//	for (const Stage& stage : valid_stages)
	//		m_defines[stage] = "";
	//	for (const DefineItem& item : defines)
	//		m_defines[item.stage] += "#define " + item.entry + "\n";
	//	return *this;
	//}

	Shader::Stage Shader::GetStage(const std::string_view& name) {
		if (name == "vertex")
			return Stage::VERTEX;
		else if (name == "geometry")
			return Stage::GEOMETRY;
		else if (name == "fragment")
			return Stage::FRAGMENT;
		else
			return Stage::INVALID;
	}

	Shader::Stage Shader::ValidateShaderStage(const std::string_view& name) {
		Stage stage = GetStage(name);
		ENSURE(stage != Stage::INVALID, std::format("Invalid ({}) Shader Stage requested in file: {}", name, m_filename));
		return stage;
	}
}