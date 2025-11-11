#include <iostream>
#include <shaderc/shaderc.hpp>

int main() {
	std::cout << "Hello, World!" << std::endl;
	system("pause");

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::string shader_source = "#version 450\nlayout(location = 0) in vec3 fragColor; layout(location = 0) out vec4 outColor; void main() { outColor = vec4(fragColor, 1.0); }";
    
	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
		shader_source.c_str(),
		shader_source.length(),
		shaderc_glsl_fragment_shader,
		"shader.frag",
		options
	);

	if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
		std::vector<uint32_t> spirv_binary(result.cbegin(), result.cend());
	}
	else {
		// Handle error: result.GetErrorMessage()
	}

	shaderc::SpvCompilationResult result2 = compiler.CompileGlslToSpv(
		shader_source.c_str(),
		shader_source.length(),
		shaderc_glsl_fragment_shader,
		"shader.frag",
		options
	);
}