#pragma once

#include "../core/hephaestus_core.hpp"
#include "../command/hephCommandPool.hpp"
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
	VkBufferCreateFlags			flags = 0;
	VkSharingMode						sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkMemoryAllocateFlags		allocateFlags = 0;
	VkMemoryPropertyFlags		propertyFlags = 0;
	//deviceMask and pQueue in context (HephMemoryAllocatorCreateInfo )
};

struct	HephImageCreateInfo {
	VkImageCreateFlags			flags = 0;
	VkImageType							imageType = VK_IMAGE_TYPE_2D;
	VkExtent3D 							extent;
	VkImageUsageFlagBits 		usage;
	VkImageLayout						layout;
	VkImageAspectFlagBits		aspect;
	VkComponentMapping 			components;
	VkSharingMode						sharingMode = VK_SHARING_MODE_CONCURRENT;
  VkSampleCountFlagBits		samples = VK_SAMPLE_COUNT_1_BIT;
  uint32_t								queueFamilyIndexCount = 0;
  const uint32_t*					pQueueFamilyIndices = nullptr;
};

class		HephMemoryAllocator {
	public:
		HephMemoryAllocator() {};

		HephResult					create(HephDevice& device);
		
		static HephResult		findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags, uint32_t* memoryType);

		HephResult					createBuffer(const HephBufferCreateInfo &createInfo, HephBufferWrapper& buffer);
		void								destroyBuffer(HephBufferWrapper &buffer);
		HephResult					createImage(const HephImageCreateInfo& createInfo, HephImageWrapper& image, HephCommandPool& cmdPool);
		void								destroyImage(HephImageWrapper &image);
		HephResult					stagingMakeAndCopy(HephBufferWrapper& buffer, void* data, size_t size, HephCommandPool& cmdPool);
    HephResult	        stagingMakeAndCopyImage(HephImageWrapper& image, VkBufferImageCopy imgRegion
                          , void* data, size_t size, HephCommandPool& cmdPool);

	private:
		HephDevice	m_device;
};
