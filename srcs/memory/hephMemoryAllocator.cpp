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

void				HephMemoryAllocator::destroyBuffer(HephBufferWrapper &buffer) {
	vkDestroyBuffer(m_device.device, buffer.buffer, m_device.pAllocationCallbacks);
	vkFreeMemory(m_device.device, buffer.memory, m_device.pAllocationCallbacks);
}

HephResult	HephMemoryAllocator::createImage(const HephImageCreateInfo& createInfo, HephImageWrapper &image, HephCommandPool& cmdPool) {
  VkImageCreateInfo imageInfo  = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .flags = createInfo.flags,
    .imageType = createInfo.imageType,
    .format = image.format,
    .extent = createInfo.extent,
    .mipLevels = 1,
    .arrayLayers = 1,
    .samples = createInfo.samples,
    .tiling = VK_IMAGE_TILING_OPTIMAL,
    .usage = createInfo.usage,
    .sharingMode = createInfo.sharingMode,
    .queueFamilyIndexCount = createInfo.queueFamilyIndexCount,
    .pQueueFamilyIndices = createInfo.pQueueFamilyIndices,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
  };
	HEPH_CHECK_RESULT(HephResult(vkCreateImage(m_device.device, &imageInfo, nullptr, &image.image)).errorFormat("Failed to begin Create image !"));

  VkMemoryRequirements  memoryReqs;
  vkGetImageMemoryRequirements(m_device.device, image.image, &memoryReqs);
  VkMemoryAllocateInfo  allocImageInfo = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .allocationSize = memoryReqs.size,
    .memoryTypeIndex = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
  };
  vkAllocateMemory(m_device.device, &allocImageInfo, nullptr, &image.memory);
  vkBindImageMemory(m_device.device, image.image, image.memory, 0);

  VkImageSubresourceRange subresourceRange = {
    .aspectMask = createInfo.aspect,
    .baseMipLevel = 0,
    .levelCount = 1,
    .baseArrayLayer = 0,
    .layerCount = 1,
  };
  {
    VkImageMemoryBarrier imageMemoryBarrier = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .srcAccessMask = VkAccessFlagBits(),
      .dstAccessMask = VK_ACCESS_MEMORY_WRITE_BIT,
      .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .newLayout = createInfo.layout,
      .image = image.image,
      .subresourceRange = subresourceRange,
    };

    const VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    const VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;

    VkCommandBuffer	cmdBuffer;
		HEPH_CHECK_RESULT(cmdPool.allocate(1, &cmdBuffer).errorFormat("Unable to allocate command Buffer! {{}}"));
		VkCommandBufferBeginInfo beginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr,
		};
		vkBeginCommandBuffer(cmdBuffer, &beginInfo);
    vkCmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, VK_FALSE, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
    vkEndCommandBuffer(cmdBuffer);
  }

  VkImageViewCreateInfo viewInfo = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .image = image.image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = image.format,
    .components = createInfo.components,
    .subresourceRange = subresourceRange,
  };
	return (HephResult(vkCreateImageView(m_device.device, &viewInfo, nullptr, &image.imageView)).errorFormat("Failed to create ImageView {}!"));
}

void  HephMemoryAllocator::destroyImage(HephImageWrapper &image) {
  vkDestroyImage(m_device.device, image.image, nullptr);
  vkFreeMemory(m_device.device, image.memory, nullptr);
  vkDestroyImageView(m_device.device, image.imageView, nullptr);
}

