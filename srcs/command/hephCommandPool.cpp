#include "hephCommandPool.hpp"

HephResult	HephCommandPool::create(HephDevice& device, HephCommandPoolCreateInfo createInfo) {
	m_device = device;
	m_flags = createInfo.flags;
	m_queueFamilyIndex = createInfo.queueFamilyIndex;
	VkCommandPoolCreateInfo poolInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = m_flags,
		.queueFamilyIndex = m_queueFamilyIndex,
	};
	return (HephResult(vkCreateCommandPool(m_device.device, &poolInfo, m_device.pAllocationCallbacks, &m_commandPool)
					, "Failed to create Command Pool ! {}"));
}

HephResult	HephCommandPool::destroy() {
	vkDestroyCommandPool(m_device.device, m_commandPool, m_device.pAllocationCallbacks);
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
