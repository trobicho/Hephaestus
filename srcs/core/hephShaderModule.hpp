#pragma once

#include "hephInstance.hpp"
#include <vector>

//PLACEHOLDER
int hephGetShaderStageFromFileExtension(std::string stage);

struct	HephShaderModuleWrapper {
	bool	load(HephDevice& device);

	std::string					filename = "";
	std::string					pName = "main";
	VkShaderStageFlags	stageFlags = 0;
	VkShaderModule			shaderModule;
};
