#include "zore/graphics/compute_shader.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

namespace zore {

	void ComputeShader::Dispatch(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) const {
		Bind();
		glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
	}

	Shader::Stage ComputeShader::ValidateShaderStage(const std::string_view& name) {
		Stage stage = GetStage(name);
		ENSURE(stage != Stage::INVALID, std::format("Invalid ({}) Shader Stage requested in file: {}", name, m_filename));
		ENSURE(stage == Stage::COMPUTE, std::format("Invalid ({}) Shader Stage requested in compute shader: {}", name, m_filename));
		return stage;
	}
}