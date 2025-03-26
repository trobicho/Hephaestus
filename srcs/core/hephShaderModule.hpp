#pragma once

#include "hephInstance.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

//PLACEHOLDER
int hephGetShaderStageFromFileExtension(std::string stage);

struct	HephShaderModuleWrapper {
	bool	load(HephDevice& device);
	void	destroy(HephDevice& device) {
		vkDestroyShaderModule(device.device, shaderModule, device.pAllocationCallbacks);
	}

	std::string					filename = "";
	std::string					pName = "main";
	VkShaderStageFlags	stageFlags = 0;
	VkShaderModule			shaderModule;
};
