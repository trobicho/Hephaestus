#include "hephComputePipeline.hpp"
#include <vulkan/vulkan_core.h>

HephResult	HephComputePipeline::create(HephDevice& device, HephDescriptorSetWrapper& descSet, HephShaderModuleWrapper& shaderModule) {
	descriptorSet = descSet;
	vkDestroyPipelineLayout(device.device, pipelineLayout, device.pAllocationCallbacks);
	vkDestroyPipeline(device.device, pipeline, device.pAllocationCallbacks);

	VkPipelineShaderStageCreateInfo shaderStageInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_COMPUTE_BIT,
		.module = shaderModule.shaderModule,
		.pName = shaderModule.pName.c_str(),
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &descriptorSet.layout,
	};
	
	HEPH_CHECK_RESULT(HephResult(vkCreatePipelineLayout(device.device, &pipelineLayoutInfo, nullptr, &pipelineLayout)
		, "failed to create pipelineLayout {{}} !"));

	VkComputePipelineCreateInfo pipelineInfo = {
		.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.stage = shaderStageInfo,
		.layout = pipelineLayout,
	};

	HEPH_CHECK_RESULT(HephResult(vkCreateComputePipelines(device.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline)
		, "failed to create compute pipeline {{}} !"));

	return (HephResult());
}
