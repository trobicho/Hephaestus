#pragma once

#include "hephInstance.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

//PLACEHOLDER

struct	HephShaderModuleWrapper {
	bool	loadFromSource(HephDevice& device);
	bool	loadFromBinary(HephDevice& device);

	std::string					sourceFilename = "";
	std::string					binaryFilename = "";
	VkShaderStageFlags	stageFlags = 0;
	VkShaderModule			shaderModule;
};
