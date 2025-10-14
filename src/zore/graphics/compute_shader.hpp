#pragma once

#include "zore/graphics/shader.hpp"

namespace zore {

	class ComputeShader : public Shader {
	public:
		ComputeShader() = default;
		ComputeShader(const std::string& filename, const std::vector<DefineItem>& defines = {}) : Shader(filename, defines) {}
		ComputeShader(const Shader&) = delete;
		ComputeShader(ComputeShader&&) = delete;
		ComputeShader& operator=(const ComputeShader&) = delete;
		ComputeShader& operator=(ComputeShader&&) = delete;
		~ComputeShader() = default;

		void Dispatch(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) const;

	private:
		Stage ValidateShaderStage(const std::string_view& name) override;
	};
}