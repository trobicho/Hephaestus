#pragma once

#include "hephInstance.hpp"

struct	HephPushConstant {
	VkPushConstantRange	range = {0, 0, 0};
	void*								pValues = nullptr;
};

struct	HephDescriptorSetUpdateInfo {
	VkDescriptorType	type;
	uint32_t					binding;
	void*							pInfo;
};

struct	HephDescriptorSetWrapper {
	void	destroy(HephDevice& device) {
		vkDestroyDescriptorSetLayout(device.device, layout, device.pAllocationCallbacks);
		layout = VK_NULL_HANDLE;
	}

	VkDescriptorSet															descriptorSet = VK_NULL_HANDLE;
	VkDescriptorSetLayout												layout = VK_NULL_HANDLE;
	std::vector<VkDescriptorSetLayoutBinding>		layoutBinds;
};
