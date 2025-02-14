#include "hephShaderModule.hpp"
#include <fstream>
#include <glslang/Public/ShaderLang.h>
#include <vulkan/vulkan_core.h>

int getShaderStageFromFileExtension(std::string stage) {
  if (stage == "vert") //for a vertex shader
    return (EShLanguage::EShLangVertex);
  else if (stage == "frag") //for a fragment shader
    return (EShLanguage::EShLangFragment);
  else if (stage == "comp") //for a compute shader
    return (EShLanguage::EShLangCompute);
  else if (stage == "rgen ") //for a ray generation shader
    return (EShLanguage::EShLangRayGen);
  else if (stage == "rint ") //for a ray intersection shader
    return (EShLanguage::EShLangIntersect);
  else if (stage == "rahit") //for a ray any hit shader
    return (EShLanguage::EShLangAnyHit);
  else if (stage == "rchit") //for a ray closest hit shader
    return (EShLanguage::EShLangClosestHit);
  else if (stage == "rmiss") //for a ray miss shader
    return (EShLanguage::EShLangMiss);
  else if (stage == "rcall") //for a ray callable shader
    return (EShLanguage::EShLangCallable);
  else if (stage == "tesc") //for a tessellation control shader
    return (EShLanguage::EShLangTessControl);
  else if (stage == "tese") //for a tessellation evaluation shader
    return (EShLanguage::EShLangTessEvaluation);
  else if (stage == "geom") //for a geometry shader
    return (EShLanguage::EShLangGeometry);
  else if (stage == "mesh") //for a mesh shader
    return (EShLanguage::EShLangMeshNV);
  else if (stage == "task") //for a task shader
    return (EShLanguage::EShLangTaskNV);
  else
		return (-1);
}

bool	readFile(const char* filename, std::vector<char>& buffer) {
	std::ifstream	file(filename, std::ios::ate);

	if (!file.is_open())
		return (false);
	size_t fileSize = (size_t)file.tellg();

	buffer.resize(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	return (true);
}

bool	createShaderModuleFromCode(HephDevice& device, VkShaderModule& shaderModule, std::vector<char> code) {
	VkShaderModuleCreateInfo info {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data()),
	};
	if (vkCreateShaderModule(device.device, &info, device.pAllocationCallbacks, &shaderModule) != VK_SUCCESS)
		return (false);
	return (true);
}

bool	HephShaderModuleWrapper::loadFromSource(HephDevice& device) {
	std::vector<char>	code;
	if (!sourceFilename.empty() && readFile(sourceFilename.c_str(), code)) {
		return (createShaderModuleFromCode(device, shaderModule, code));
	}
	return (false);
}

bool	HephShaderModuleWrapper::loadFromBinary(HephDevice& device) {
	std::vector<char>	code;
	if (!binaryFilename.empty() && readFile(binaryFilename.c_str(), code)) {
		return (createShaderModuleFromCode(device, shaderModule, code));
	}
	return (false);
}
