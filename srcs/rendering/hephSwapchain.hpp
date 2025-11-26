#pragma once

#include "../core/hephaestus_core.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

struct	HephSurfaceSupportDetails {
	HephSurfaceSupportDetails(){};
	HephSurfaceSupportDetails(const HephDevice& device, VkSurfaceKHR surface) {getSurfaceSupportDetails(device, surface);}
	VkSurfaceCapabilitiesKHR					capabilies;
	std::vector<VkSurfaceFormatKHR>		formats;
	std::vector<VkPresentModeKHR>			presentModes;

	HephResult		getSurfaceSupportDetails(const HephDevice& device, VkSurfaceKHR surface) {
		return (getSurfaceSupportDetails(device.physicalDevice, surface));
	}
	HephResult		getSurfaceSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		uint32_t	count;
		HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilies)
					, "Failed to get device surface capabilities!"));
		HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr)
					, "Failed to get device surface formats!"));
		if (count > 0) {
			formats.resize(count);
			HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data())
						, "Failed to get device surface formats!"));
		}
		HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr)
					, "Failed to get device surface present modes!"));
		if (count > 0) {
			presentModes.resize(count);
			HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, presentModes.data())
						, "Failed to get device surface present modes!"));
		}
		return (HephResult());
	}
};

struct	HephSwapchainImage {
	HephSwapchainImage() {};
	VkImage					image = VK_NULL_HANDLE;
	VkImageView			imageView = VK_NULL_HANDLE;
	VkFramebuffer		framebuffer = VK_NULL_HANDLE;
};

struct	HephSwapchainSync {
	VkSemaphore			semaphoreAvailable = VK_NULL_HANDLE;
	VkSemaphore			semaphoreFinish = VK_NULL_HANDLE;
	VkFence					fence = VK_NULL_HANDLE;
};

struct	HephSwapchainCreateInfo {
	VkSwapchainCreateInfoKHR	swapchainCreateInfo;
	VkRenderPass 							renderPass = VK_NULL_HANDLE;
	VkComponentMapping				componentMapping = (VkComponentMapping){
		.r = VK_COMPONENT_SWIZZLE_IDENTITY,
		.g = VK_COMPONENT_SWIZZLE_IDENTITY,
		.b = VK_COMPONENT_SWIZZLE_IDENTITY,
		.a = VK_COMPONENT_SWIZZLE_IDENTITY,
	};
	VkImageSubresourceRange		imageSubresourceRange = (VkImageSubresourceRange) {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};
};

struct  HephSwapchainPresentData {
	HephSwapchainSync		syncObject;
	HephSwapchainImage	image;
  VkExtent2D      		extent;
	uint32_t						imageCurrent;
  uint32_t        		imageIndex;
	VkSwapchainKHR			swapchain;
};

class		HephSwapchain {
	public:
		HephSwapchain() {};
		~HephSwapchain() {};

		HephResult	recreate(VkExtent2D extent, VkSurfaceKHR surface); 
		HephResult	create(HephDevice& device, HephSwapchainCreateInfo createInfo); 
		HephResult	createFramebuffers(VkRenderPass renderPass);
		HephResult	acquireNextImage(HephSwapchainPresentData& presentData);
		HephResult	destroy();

		uint32_t		getImageCount() {return(m_imageCount);}

	private:
		HephResult	createSwapchain();
		HephResult	destroySwapImage(HephSwapchainImage &image) {
			vkDestroyImageView(m_device.device, image.imageView, m_device.pAllocationCallbacks);
      if (image.framebuffer != VK_NULL_HANDLE)
        vkDestroyFramebuffer(m_device.device, image.framebuffer, m_device.pAllocationCallbacks);
			image = HephSwapchainImage();
			return (HephResult());
		}
		HephResult	destroySyncObject(HephSwapchainSync &sync) {
			vkDestroySemaphore(m_device.device, sync.semaphoreAvailable, m_device.pAllocationCallbacks);
			vkDestroySemaphore(m_device.device, sync.semaphoreFinish, m_device.pAllocationCallbacks);
			vkDestroyFence(m_device.device, sync.fence, m_device.pAllocationCallbacks);
			sync = HephSwapchainSync();
			return (HephResult());
		}
		HephResult	setExtent(VkExtent2D extent) {
			m_extent = extent;
			return (createSwapchain());
		}

		HephSwapchainCreateInfo						m_createInfo;
		VkSwapchainKHR										m_swapchain = VK_NULL_HANDLE;
		HephDevice												m_device;
		uint32_t													m_imageCount = 0;
		uint32_t													m_imageCurrent = 0;
		std::vector<HephSwapchainImage>		m_images;
		std::vector<HephSwapchainSync>		m_syncObjects;
		VkExtent2D												m_extent;
};
