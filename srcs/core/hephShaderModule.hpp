#pragma once

#include "hephInstance.hpp"
#include <vector>

//PLACEHOLDER
int hephGetShaderStageFromFileExtension(std::string stage);

struct	HephShaderModuleWrapper {
	bool	loadFromSource(HephDevice& device);
	bool	loadFromBinary(HephDevice& device);

	std::string					sourceFilename = "";
	std::string					binaryFilename = "";
	std::string					pName = "main";
	VkShaderStageFlags	stageFlags = 0;
	VkShaderModule			shaderModule;
};
