#include "hephGui.hpp"
#include "config_app.h"
#include <cstddef>

HephResult	HephGui::createPipelines() {

	auto	layouts = m_pipelineDescriptor.getLayoutBuffer();
  VkPipelineLayoutCreateInfo 				pipelineLayoutInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = static_cast<uint32_t>(layouts.size()),
    .pSetLayouts = layouts.data(),
    .pushConstantRangeCount = 0, // Optional
    .pPushConstantRanges = nullptr, // Optional
  };

  HEPH_CHECK_RESULT(HephResult(vkCreatePipelineLayout(m_device.device, &pipelineLayoutInfo
        , nullptr, &m_pipelineLayout), "failed to create pipeline layout!"));

  VkVertexInputBindingDescription       vertexBindingDesc = {
    .binding = 0,
    .stride = sizeof(HephVertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
  };

  VkVertexInputAttributeDescription     vertexAttrDesc[2] = {
    {
      .location = 0,
      .binding = 0,
      .format = VK_FORMAT_R32G32B32_SFLOAT,
      .offset = 0,
    },
    {
      .location = 1,
      .binding = 0,
      .format = VK_FORMAT_R32G32B32A32_SFLOAT,
      .offset = offsetof(HephVertex, color),
    }
  };

  VkPipelineVertexInputStateCreateInfo    vertexInputInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &vertexBindingDesc,
    .vertexAttributeDescriptionCount = 2,
    .pVertexAttributeDescriptions = vertexAttrDesc,
  };

  VkPipelineInputAssemblyStateCreateInfo  inputAssemblyInfoLine = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    .primitiveRestartEnable = VK_FALSE,
  };
  VkPipelineInputAssemblyStateCreateInfo  inputAssemblyInfoTri = {
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
	vertShaderModule.load(m_device);
	fragShaderModule.load(m_device);

  VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
    VK_DYNAMIC_STATE_LINE_WIDTH,
  };

  VkPipelineDynamicStateCreateInfo  dynamicStateInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .dynamicStateCount = 3,
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
    .pInputAssemblyState = &inputAssemblyInfoLine,
    .pViewportState = &viewportInfo,
    .pRasterizationState = &rasterizationInfo,
    .pMultisampleState = &multisampleStateInfo,
    .pDepthStencilState = nullptr, // Optional
    .pColorBlendState = &colorBlendInfo,
    .pDynamicState = &dynamicStateInfo,
    .layout = m_pipelineLayout,
    .renderPass = m_renderPass,
  };

  HEPH_CHECK_RESULT(HephResult(vkCreateGraphicsPipelines(m_device.device, VK_NULL_HANDLE, 1,
        &pipelineInfo, nullptr, &m_pipelineLine), "failed to create graphics pipeline!"));

  pipelineInfo.pInputAssemblyState = &inputAssemblyInfoTri;
  HEPH_CHECK_RESULT(HephResult(vkCreateGraphicsPipelines(m_device.device, VK_NULL_HANDLE, 1,
        &pipelineInfo, nullptr, &m_pipelineTri), "failed to create graphics pipeline!"));

	vertShaderModule.destroy(m_device);
	fragShaderModule.destroy(m_device);
	return (HephResult());
}

/*
void 				GraphWindow::updateDescriptorSets() {
  m_memoryAllocator.stagingMakeAndCopy(m_cameraUbo, &m_camera, sizeof(Camera), m_commandPool);
  VkDescriptorBufferInfo		bufferInfo = {
		.buffer = m_cameraUbo.buffer,
		.offset = 0,
		.range = VK_WHOLE_SIZE,
  };
	HephDescriptorUpdateInfo	updateInfo[] = {
		{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.binding = 0,
			.pInfo = &bufferInfo,
		},
	};
	m_pipelineDescriptor.update(m_device, 0, updateInfo, 1);
}
*/
