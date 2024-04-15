#pragma once

#include "../core/hephaestus_core.hpp"
#include <vulkan/vulkan_core.h>

struct	HephImageWrapper {
	VkImage							image = VK_NULL_HANDLE;
	VkImageUsageFlags		usage;
	VkImageLayout				layout = VK_IMAGE_LAYOUT_GENERAL;
	VkExtent3D					extent = {0, 0, 0};
	VkFormat						format;
	VkImageView					imageView = VK_NULL_HANDLE;
	VkDeviceMemory			memory = VK_NULL_HANDLE;
	VkDeviceSize				size;
};

struct	HephBufferWrapper {
	VkBuffer						buffer = VK_NULL_HANDLE;
	VkDeviceMemory			memory = VK_NULL_HANDLE;
	VkBufferUsageFlags	usage = 0;
	VkDeviceSize				size = 0;
};

struct	HephBufferCreateInfo {
	VkDeviceSize						size = 0;
	VkBufferUsageFlags			usage = 0;
	VkSharingMode						sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkMemoryAllocateFlags		allocateFlags = 0;
	VkMemoryPropertyFlags		propertyFlags = 0;
	//deviceMask and pQueue in context (HephMemoryAllocatorCreateInfo )
};

class		HephMemoryAllocator {
	public:
		HephMemoryAllocator() {};

		HephResult					create(HephDevice& device);
		
		static HephResult		findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags, uint32_t* memoryType);

		HephResult					createBuffer(const HephBufferCreateInfo &createInfo, HephBufferWrapper &buffer);
		void								destroyBuffer(HephBufferWrapper &buffer);

	private:
		HephDevice	m_device;
};
