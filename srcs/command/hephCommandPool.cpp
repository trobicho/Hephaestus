#include "hephCommandPool.hpp"
#include <vulkan/vulkan_core.h>

HephResult	HephCommandPool::create(HephDevice& device, HephCommandPoolCreateInfo createInfo) {
	m_device = device;
	m_flags = createInfo.flags;
  m_queue = createInfo.queue;
  if (m_queue == nullptr)
    m_queue = &m_device.queues[0];
	VkCommandPoolCreateInfo poolInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = m_flags,
		.queueFamilyIndex = m_queue->familyIndex,
	};
	return (HephResult(vkCreateCommandPool(m_device.device, &poolInfo, m_device.pAllocationCallbacks, &m_commandPool)
					, "Failed to create Command Pool ! {}"));
}

HephResult	HephCommandPool::destroy() {
	if (m_commandPool != VK_NULL_HANDLE)
		vkDestroyCommandPool(m_device.device, m_commandPool, m_device.pAllocationCallbacks);
	m_commandPool = VK_NULL_HANDLE;
	return (HephResult());
}

HephResult	HephCommandPool::allocate(uint32_t size, VkCommandBuffer* commandBufferPtr, VkCommandBufferLevel level) {
	HEPH_CHECK_RESULT(HephResult("HephCommandPool: !(size > 0)", (size > 0)));
	VkCommandBufferAllocateInfo allocateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = m_commandPool,
		.level = level,
		.commandBufferCount = size,
	};
	return (HephResult(vkAllocateCommandBuffers(m_device.device, &allocateInfo, commandBufferPtr)
					, "Failed to allocate Command Buffers ! {}"));
}

void				HephCommandPool::submit(VkCommandBuffer& cmdBuffer) {
  VkSubmitInfo info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .commandBufferCount = 1,
    .pCommandBuffers = &cmdBuffer,
  };
  vkQueueSubmit(m_queue->queue, 1, &info, VK_NULL_HANDLE);
}

HephResult	HephCommandPool::submitAndWait(VkCommandBuffer& cmdBuffer) {
  submit(cmdBuffer);
  return (HephResult(vkQueueWaitIdle(m_queue->queue)).errorFormat("Failed to submit and wait: {{}} !"));
}
