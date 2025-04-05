#pragma once

#include "hephInstance.hpp"

struct	HephPushConstant {
	VkPushConstantRange	range = {0, 0, 0};
	void*								pValues = nullptr;
};

struct  HephDescriptorUpdateInfo {
  VkDescriptorType  type;
  uint32_t          binding;
  void*             pInfo = nullptr;
};


struct	HephDescriptorWrapper {
	void	destroy(HephDevice& device) {
		vkDestroyDescriptorSetLayout(device.device, layout, device.pAllocationCallbacks);
		layout = VK_NULL_HANDLE;
	}

	VkDescriptorSet															set = VK_NULL_HANDLE;
	VkDescriptorSetLayout												layout = VK_NULL_HANDLE;
	std::vector<VkDescriptorSetLayoutBinding>		layoutBinds;
};

struct	HephPipelineDescriptor {
	HephResult	build(HephDevice& device);
	void				update(HephDevice& device, uint32_t id, const HephDescriptorUpdateInfo* info, uint32_t count);
	void				destroy(HephDevice& device) {
		vkDestroyDescriptorPool(device.device, descriptorPool, device.pAllocationCallbacks);
		for (auto& descriptor: descriptors)
			descriptor.destroy(device);
	}
	std::vector<VkDescriptorSetLayout>	getLayoutBuffer() {
		std::vector<VkDescriptorSetLayout>	layouts;
		for (auto& descriptor: descriptors)
			layouts.push_back(descriptor.layout);
		return (layouts);
	};
	std::vector<VkDescriptorSet>	getSetBuffer() {
		std::vector<VkDescriptorSet>	sets;
		for (auto& descriptor: descriptors)
			sets.push_back(descriptor.set);
		return (sets);
	};


	std::vector<HephDescriptorWrapper>	descriptors;
	VkDescriptorPool										descriptorPool;
};
