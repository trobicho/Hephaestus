#include "hephMemoryAllocator.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

HephResult	HephMemoryAllocator::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter
		, VkMemoryPropertyFlags propertyFlags, uint32_t* memoryType)
{
  VkPhysicalDeviceMemoryProperties  memProps;

  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
  for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
    if (typeFilter & (1 << i) && (memProps.memoryTypes[i].propertyFlags & propertyFlags)) {
			if (memoryType != nullptr) {
				*memoryType = i;
				return (HephResult());
			}
		}
  }
	return (HephResult("Unable to find Memory types"));
}

HephResult	HephMemoryAllocator::create(HephDevice& device) {
	m_device = device;
	return (HephResult());
}

HephResult	HephMemoryAllocator::createBuffer(const HephBufferCreateInfo &createInfo, HephBufferWrapper &buffer) {
	VkBufferCreateInfo	bufferInfo = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.pNext = nullptr,
		.flags = createInfo.propertyFlags,
		.size = createInfo.size,
		.usage = createInfo.usage,
		.sharingMode = createInfo.sharingMode,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr,
	};
	HEPH_CHECK_RESULT(HephResult(vkCreateBuffer(m_device.device, &bufferInfo, m_device.pAllocationCallbacks, &buffer.buffer)
				, "Unable to create buffer {}"));

  VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_device.device, buffer.buffer, &memRequirements);

  VkMemoryAllocateFlagsInfo allocateFlags = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
		.pNext = nullptr,
    .flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT,
		.deviceMask = 0,
  };

	uint32_t	memoryType = 0;
	HEPH_CHECK_RESULT(findMemoryType(m_device.physicalDevice, memRequirements.memoryTypeBits, createInfo.propertyFlags, &memoryType)
			.errorFormat("Unable to allocate buffer {}!"));

  VkMemoryAllocateInfo memAllocInfo = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = &allocateFlags,
    .allocationSize = memRequirements.size,
    .memoryTypeIndex = memoryType,
  };
  HEPH_CHECK_RESULT(HephResult(vkAllocateMemory(m_device.device, &memAllocInfo, m_device.pAllocationCallbacks, &buffer.memory)
				, "Unable to allocate buffer memory!"));

  vkBindBufferMemory(m_device.device, buffer.buffer, buffer.memory, 0);
	return (HephResult());
}

void	HephMemoryAllocator::destroyBuffer(HephBufferWrapper &buffer) {
	vkDestroyBuffer(m_device.device, buffer.buffer, m_device.pAllocationCallbacks);
	vkFreeMemory(m_device.device, buffer.memory, m_device.pAllocationCallbacks);
}
