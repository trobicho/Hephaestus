#pragma once

#include "../core/hephaestus_core.hpp"

struct	HephSurfaceSupportDetails {
	VkSurfaceCapabilitiesKHR					capabilies;
	std::vector<VkSurfaceFormatKHR>		formats;
	std::vector<VkPresentModeKHR>			presentModes;
	static HephResult		getSurfaceSupportDetails(const HephDevice& device, VkSurfaceKHR surface, HephSurfaceSupportDetails& details) {
		return (getSurfaceSupportDetails(device.physicalDevice, surface, details));
	}
	static HephResult		getSurfaceSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, HephSurfaceSupportDetails& details) {
		uint32_t	count;
		HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilies)
					, "Failed to get device surface capabilities!"));
		HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr)
					, "Failed to get device surface formats!"));
		if (count > 0) {
			details.formats.resize(count);
			HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, details.formats.data())
						, "Failed to get device surface formats!"));
		}
		HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr)
					, "Failed to get device surface present modes!"));
		if (count > 0) {
			details.presentModes.resize(count);
			HEPH_CHECK_RESULT(HephResult(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, details.presentModes.data())
						, "Failed to get device surface present modes!"));
		}
		return (HephResult());
	}
};

class		HephSwapchain {
	public:
		HephSwapchain() {};


};
