#pragma once

#include "hephPipeline.hpp"

struct	HephComputePipeline : HephPipeline {
		HephComputePipeline() {};
		~HephComputePipeline() {};

		void				destroy(HephDevice& device) {
			vkDestroyPipelineLayout(device.device, pipelineLayout, device.pAllocationCallbacks);
			vkDestroyPipeline(device.device, pipeline, device.pAllocationCallbacks);
			pipeline = VK_NULL_HANDLE;
			pipelineLayout = VK_NULL_HANDLE;
		};

		HephResult	create(HephDevice& device, HephDescriptorSetWrapper& descSet, HephShaderModuleWrapper& shaderModule);

		int					type(){return (HEPH_PIPELINE_TYPE_COMPUTE);}

		VkPipeline								pipeline = VK_NULL_HANDLE;
		VkPipelineLayout					pipelineLayout = VK_NULL_HANDLE;
		HephDescriptorSetWrapper	descriptorSet;
};
