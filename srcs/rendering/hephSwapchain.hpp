#pragma once

#include "../core/hephaestus_core.hpp"
#include <vulkan/vulkan_core.h>

struct	HephSurfaceSupportDetails {
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

struct	HephSwapchainCreateInfo {
	VkSwapchainCreateInfoKHR	swapchainCreateInfo;
	VkRenderPass 							renderPass = VK_NULL_HANDLE;
	VkComponentMapping				componenMapping = (VkComponentMapping){
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

class		HephSwapchain {
	public:
		HephSwapchain() {};
		~HephSwapchain() {destroy();}

		HephResult	create(HephDevice& device, HephSwapchainCreateInfo createInfo); 
		HephResult	createFramebuffers(VkRenderPass renderPass);
		HephResult	destroy();

	private:
		HephResult	createSwapchain();

		HephSwapchainCreateInfo						m_createInfo;
		VkSwapchainKHR										m_swapchain = VK_NULL_HANDLE;
		HephDevice												m_device;
		uint32_t													m_imageCount = 0;
		std::vector<HephSwapchainImage>		m_images;
		VkExtent2D												m_extent;
};
