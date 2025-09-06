#pragma once

#include "../core/hephaestus_core.hpp"
#include <vulkan/vulkan_core.h>

struct	HephCommandPoolCreateInfo {
	VkCommandPoolCreateFlagBits 	flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	uint32_t											queueFamilyIndex;
};

class		HephCommandPool : private HephCommandPoolCreateInfo {
	public:
		HephCommandPool() {};
		~HephCommandPool() {destroy();}

		HephResult	create(HephDevice& device, HephCommandPoolCreateInfo createInfo);
		HephResult	destroy();
		HephResult	allocate(uint32_t size, VkCommandBuffer* commandBufferPtr, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    void        free(VkCommandBuffer& cmdBuffer) {vkFreeCommandBuffers(m_device.device, m_commandPool, 1, &cmdBuffer);}
    void				submit(VkCommandBuffer& cmdBuffer);
    HephResult	submitAndWait(VkCommandBuffer& cmdBuffer);

	private:
		HephDevice										m_device;
		VkCommandPool									m_commandPool;
		VkCommandPoolCreateFlagBits 	m_flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		uint32_t											m_queueFamilyIndex;
};
