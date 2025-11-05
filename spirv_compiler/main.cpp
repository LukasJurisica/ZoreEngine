#include <iostream>
#include <shaderc/shaderc.hpp>

int main() {
	std::cout << "Hello, World!" << std::endl;
	system("pause");

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
		shader_source.c_str(),
		shader_source.length(),
		shaderc_glsl_vertex_shader,
		"shader.vert",
		options
	);

	if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
		std::vector<uint32_t> spirv_binary(result.cbegin(), result.cend());
	}
	else {
		// Handle error: result.GetErrorMessage()
	}
}