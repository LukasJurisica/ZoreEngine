#include "zore/graphics/shader.hpp"
#include "zore/graphics/devices/render_device.hpp"
#include "zore/core/file_manager.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"
#include <vulkan/vulkan.h>
#include <shaderc/shaderc.hpp>

namespace zore {

	static int s_version = 460;
	static shaderc_profile s_profile = shaderc_profile::shaderc_profile_core;

	//========================================================================
	//	Shader
	//========================================================================

	Shader::Shader(const std::string& filename, const std::vector<Define>& defines) : m_filename(filename) {
		Compile();
	}

	Shader::~Shader() {
		for (const auto& [stage, module] : m_shader_stages)
			vkDestroyShaderModule(RenderDevice::Get(), module.as<VkShaderModule>(), nullptr);
	}

	void Shader::SetShaderVersion(int major, int minor, bool core) {
		s_version = ((major * 10) + minor) * 10;
		s_profile = core ? shaderc_profile::shaderc_profile_core : shaderc_profile::shaderc_profile_compatibility;
	}

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

	void Shader::Compile() {
		std::string source;
		FileManager::ReadContent(source, s_shader_folder + m_filename, IS_DEBUG);
		size_t right = source.size();
		size_t left = source.rfind("#shaderstage", right);

		// Parse Shader file, and create shader stages
		while (left != std::string::npos) {
			CreateShaderStage(std::string_view(source.data() + left, right - left));
			source[left] = '\0';
			right = left - 1;
			left = source.rfind("#shaderstage", right);
		}

		Logger::Log("SHADER COMPILED SUCCESSFULLY.");
	}

	void Shader::CreateShaderStage(const std::string_view& content) {
		size_t start = content.find("\n", 0);
		std::string_view stage_name = std::string_view(content.data() + 13, start - 13);
		std::string_view source = std::string_view(content.data() + start + 1, content.size() - start - 1);
		Shader::Stage stage = ValidateShaderStage(stage_name);

		static const shaderc_shader_kind s_vk_module_types[] = {
			shaderc_shader_kind::shaderc_glsl_vertex_shader,
			shaderc_shader_kind::shaderc_glsl_geometry_shader,
			shaderc_shader_kind::shaderc_glsl_fragment_shader,
			shaderc_shader_kind::shaderc_glsl_compute_shader,
			shaderc_shader_kind::shaderc_glsl_infer_from_source
		};

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetOptimizationLevel(shaderc_optimization_level_performance);
		options.SetForcedVersionProfile(s_version, s_profile);
		options.SetVulkanRulesRelaxed(true);
		options.SetInvertY(true);
		//options.AddMacroDefinition("ZORE_SHADER", "true");

		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
			source.data(),
			source.length(),
			s_vk_module_types[static_cast<uint32_t>(stage)],
			m_filename.c_str(),
			options
		);

		ENSURE(result.GetCompilationStatus() == shaderc_compilation_status_success, std::format("Error compiling {} shader: {}\n{}", stage_name, m_filename, result.GetErrorMessage()));
		std::vector<uint32_t> spirv_binary(result.cbegin(), result.cend());

		VkShaderModuleCreateInfo shader_module_info{};
		shader_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_module_info.codeSize = spirv_binary.size() * sizeof(uint32_t);
		shader_module_info.pCode = spirv_binary.data();

		VkShaderModule shader_module;
		ENSURE(vkCreateShaderModule(RenderDevice::Get(), &shader_module_info, nullptr, &shader_module) == VK_SUCCESS, std::format("Error creating {} shader module for {}.", stage_name, m_filename));
		m_shader_stages.emplace_back(std::make_pair(stage, shader_module));
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