#include "hephGuiInternal.hpp"
#include "config_app.h"
#include <vulkan/vulkan_core.h>

namespace HephGui {

HephResult	HephGuiContext::createPipeline() {

	auto	layouts = pipelineDescriptor.getLayoutBuffer();

  VkPushConstantRange               pushConstantRange = {
    .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    .offset = 0,
    .size = 2 * sizeof(glm::vec2),
  };

  VkPipelineLayoutCreateInfo 				pipelineLayoutInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = static_cast<uint32_t>(layouts.size()),
    .pSetLayouts = layouts.data(),
    .pushConstantRangeCount = 1, // Optional
    .pPushConstantRanges = &pushConstantRange,
  };

  HEPH_CHECK_RESULT(HephResult(vkCreatePipelineLayout(device.device, &pipelineLayoutInfo
        , nullptr, &pipelineLayout), "failed to create pipeline layout!"));

  VkVertexInputBindingDescription       vertexBindingDesc = {
    .binding = 0,
    .stride = sizeof(HephVertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
  };

  VkVertexInputAttributeDescription     vertexAttrDesc[] = {
    {
      .location = 0,
      .binding = 0,
      .format = VK_FORMAT_R32G32_SFLOAT,
      .offset = 0,
    },
    {
      .location = 1,
      .binding = 0,
      .format = VK_FORMAT_R32G32_SFLOAT,
      .offset = offsetof(HephVertex, uv),
    },
    {
      .location = 2,
      .binding = 0,
      .format = VK_FORMAT_R32G32B32A32_SFLOAT,
      .offset = offsetof(HephVertex, color),
    }
  };

  VkPipelineVertexInputStateCreateInfo    vertexInputInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &vertexBindingDesc,
    .vertexAttributeDescriptionCount = 3,
    .pVertexAttributeDescriptions = vertexAttrDesc,
  };

  VkPipelineInputAssemblyStateCreateInfo  inputAssemblyInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = VK_FALSE,
  };

  VkPipelineRasterizationStateCreateInfo rasterizationInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_BACK_BIT,
    .frontFace = VK_FRONT_FACE_CLOCKWISE,
    .depthBiasEnable = VK_FALSE,
    .depthBiasConstantFactor = 0.0f, // Optional
    .depthBiasClamp = 0.0f, // Optional
    .depthBiasSlopeFactor = 0.0f, // Optional
    .lineWidth = 1.0f,
  };

  VkPipelineViewportStateCreateInfo   viewportInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .viewportCount = 1,
    .scissorCount = 1,
  };

  VkPipelineMultisampleStateCreateInfo multisampleStateInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .minSampleShading = 1.0f, // Optional
    .pSampleMask = nullptr, // Optional
    .alphaToCoverageEnable = VK_FALSE, // Optional
    .alphaToOneEnable = VK_FALSE, // Optional
  };

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {
    .blendEnable = VK_TRUE,
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .colorBlendOp = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp = VK_BLEND_OP_ADD,
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
  };

  VkPipelineColorBlendStateCreateInfo colorBlendInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .logicOpEnable = VK_FALSE,
    .attachmentCount = 1,
    .pAttachments = &colorBlendAttachment,
  };

  HephShaderModuleWrapper	vertShaderModule = {
		.filename = std::string(APP_ROOT) + "/spv/tri.vert.spv",
	};
  HephShaderModuleWrapper	fragShaderModule = {
		.filename = std::string(APP_ROOT) + "/spv/tri.frag.spv",
	};
	vertShaderModule.load(device);
	fragShaderModule.load(device);

  VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
  };

  VkPipelineDynamicStateCreateInfo  dynamicStateInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .dynamicStateCount = 2,
    .pDynamicStates = dynamicStates,
  };

  VkPipelineShaderStageCreateInfo   vertShaderStageInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_VERTEX_BIT,
    .module = vertShaderModule.shaderModule,
    .pName = "main", //TODO: entrypoint
    .pSpecializationInfo = nullptr,
  };
  VkPipelineShaderStageCreateInfo   fragShaderStageInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
    .module = fragShaderModule.shaderModule,
    .pName = "main", //TODO: entrypoint
    .pSpecializationInfo = nullptr,
  };
  VkPipelineShaderStageCreateInfo   shaderStages[] = {
    vertShaderStageInfo,
    fragShaderStageInfo,
  };

  VkGraphicsPipelineCreateInfo  		pipelineInfo = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .stageCount = 2,
    .pStages = shaderStages,
    .pVertexInputState = &vertexInputInfo,
    .pInputAssemblyState = &inputAssemblyInfo,
    .pViewportState = &viewportInfo,
    .pRasterizationState = &rasterizationInfo,
    .pMultisampleState = &multisampleStateInfo,
    .pDepthStencilState = nullptr, // Optional
    .pColorBlendState = &colorBlendInfo,
    .pDynamicState = &dynamicStateInfo,
    .layout = pipelineLayout,
    .renderPass = renderPass,
  };

  HEPH_CHECK_RESULT(HephResult(vkCreateGraphicsPipelines(device.device, VK_NULL_HANDLE, 1,
        &pipelineInfo, nullptr, &pipeline), "failed to create graphics pipeline!"));

	vertShaderModule.destroy(device);
	fragShaderModule.destroy(device);
	return (HephResult());
}

void        HephGuiContext::updateDescriptorSets() {
  VkDescriptorImageInfo imageInfo = {
    .sampler = sampler,
    .imageView = font.getTextureAtlas().image.imageView,
    .imageLayout = font.getTextureAtlas().image.layout,
  };
	HephDescriptorUpdateInfo	updateInfo[] = {
		{
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.binding = 0,
			.pInfo = &imageInfo,
		},
	};
	pipelineDescriptor.update(device, 0, updateInfo, 1);
}

HephResult  HephGuiContext::create(HephDevice& device_a, VkRenderPass renderPass_a) {
  device = device_a;
  renderPass = renderPass_a;
	memoryAllocator.create(device);
  displayPos.x = 0;
  displayPos.y = 0;
	HephCommandPoolCreateInfo	cmdPoolCreateInfo = {
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = device.queues[0].familyIndex,
	};
	HEPH_CHECK_RESULT(commandPool.create(device, cmdPoolCreateInfo));

  HephFontCreateInfo  fontCreateInfo = {
    .fontFilePath = "/usr/share/fonts/TTF/AgaveNerdFont-Regular.ttf",
    .faceIndex = 0,
    .pixelSize = 18,
    .mAllocator = &memoryAllocator,
    .cmdPool = &commandPool,
  };
  HEPH_CHECK_RESULT(font.load(fontCreateInfo));
  sharedData.font = &font;
  sharedData.whitePixelArea = font.getTextureAtlas().whitePixelArea;

  HephDescriptorWrapper	descriptor = {
    .layoutBinds = {
      (VkDescriptorSetLayoutBinding) {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
      },
    }
  };
	pipelineDescriptor.descriptors.push_back(descriptor);
  HEPH_CHECK_RESULT(pipelineDescriptor.build(device).errorFormat("Unable To Build Pipeline Descriptor {}"));
	HEPH_CHECK_RESULT(createPipeline());

  VkSamplerCreateInfo samplerInfo{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  HEPH_CHECK_RESULT(HephResult(vkCreateSampler(device.device, &samplerInfo, nullptr, &sampler), "Failed to create Sampler !"));
  updateDescriptorSets();

  return (HephResult());
}

void        HephGuiContext::destroy() {
  memoryAllocator.destroyBuffer(vertexBuffer);
  memoryAllocator.destroyBuffer(indexBuffer);
  commandPool.destroy();
  vkDestroyPipeline(device.device, pipeline, device.pAllocationCallbacks);
  vkDestroyPipelineLayout(device.device, pipelineLayout, device.pAllocationCallbacks);
  pipeline = VK_NULL_HANDLE;
  pipelineLayout = VK_NULL_HANDLE;
  pipelineDescriptor.destroy(device);
}

}
