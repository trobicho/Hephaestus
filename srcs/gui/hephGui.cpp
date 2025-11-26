#include "hephGui.hpp"

namespace HephGui {

HephResult   HephGuiContext::init() {
  return (HephPluginFont::init());
}

HephResult   HephGuiContext::create(HephDevice& device, VkRenderPass renderPass) {
  m_device = device;
  m_renderPass = renderPass;
	m_memoryAllocator.create(m_device);
	HephCommandPoolCreateInfo	cmdPoolCreateInfo = {
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = m_device.queues[0].familyIndex,
	};
	HEPH_CHECK_RESULT(m_commandPool.create(m_device, cmdPoolCreateInfo));
	HephDescriptorWrapper	descriptor = {
		.layoutBinds = {
			(VkDescriptorSetLayoutBinding) {
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			},
		}
  };
	m_pipelineDescriptor.descriptors.push_back(descriptor);
  HEPH_CHECK_RESULT(m_pipelineDescriptor.build(m_device).errorFormat("Unable To Build Pipeline Descriptor {}"));
	HEPH_CHECK_RESULT(createPipelines());
  return (HephResult());
};

}
