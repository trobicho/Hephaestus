#pragma once

#include "../core/hephaestus_core.hpp"

struct	HephComputePipeline {
		HephComputePipeline() {};
		~HephComputePipeline() {};

		void	destroy(HephDevice& device) {
			vkDestroyPipelineLayout(device.device, pipelineLayout, device.pAllocationCallbacks);
			vkDestroyPipeline(device.device, pipeline, device.pAllocationCallbacks);
			pipeline = VK_NULL_HANDLE;
			pipelineLayout = VK_NULL_HANDLE;
		};

		HephResult	create(HephDevice& device, HephDescriptorSetWrapper& descSet, HephShaderModuleWrapper& shaderModule);

		VkPipeline								pipeline = VK_NULL_HANDLE;
		VkPipelineLayout					pipelineLayout = VK_NULL_HANDLE;
		HephDescriptorSetWrapper	descriptorSet;
};
