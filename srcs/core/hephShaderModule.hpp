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

	std::string					  filename = "";
	std::string					  pName = "main";
	VkShaderStageFlagBits	stageFlagBits;
	VkShaderModule			  shaderModule;
};

struct	HephShaderStageWrapper: public HephShaderModuleWrapper {
  HephShaderStageWrapper() = default;
  HephShaderStageWrapper(const HephShaderModuleWrapper& module) {
    filename = module.filename;
    pName = module.pName;
    stageFlagBits = module.stageFlagBits;
    shaderModule = module.shaderModule;
  }

  VkPipelineShaderStageCreateInfo stageCreateInfo;
};
