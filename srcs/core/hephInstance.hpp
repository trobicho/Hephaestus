#pragma once

#include "hephResult.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

struct	HephQueue {
	VkQueue				queue;
	VkQueueFlags	flags;
	bool					inUse = false;
};

struct	HephQueueReserveInfo {
	VkQueueFlags	flags;
	uint32_t			reserveCount;
};

class		HephInstanceBase {
	public:
		VkInstance				vulkanInstance;
		VkDevice					device;
		VkPhysicalDevice	physicalDevice;
};

struct HephInstanceCreateInfo {
	const char*										pApplicationName;	
	const char*										pEngineName = "";	
	const char**									ppHephInstanceExtensions;
	const uint32_t								hephInstanceExtensionCount;

	const char**									ppVkInstanceExtensions;
	const uint32_t								vkInstanceExtensionCount;
	const char**									ppVkValidationLayers;
	const uint32_t								vkValidationLayerCount;
};

struct HephDeviceCreateInfo {
	const HephQueueReserveInfo*				pQueueReserveInfos;
	const uint32_t										queueReserveInfoCount;
	const char**											ppHephDeviceExtensions;
	const uint32_t										hephDeviceExtensionCount;

	const char**											ppVkDeviceExtensions;
	const uint32_t										vkDeviceExtensionCount;
	const char**											ppVkValidationLayers;
	const uint32_t										vkValidationLayerCount;
	const VkPhysicalDeviceFeatures*		pVkPhysicalDeviceFeatures;
	const VkPhysicalDeviceFeatures2*	pVkPhysicalDeviceFeatures2;
};

struct	HephInitializationInfo {
	VkDevice					device;
	VkQueue						queue;
};

class		HephInstance: public HephInstanceBase {
	public:
		HephInstance() {};
		HephResult	destroy();

		HephResult	createInstance(HephInstanceCreateInfo& createInfo);
		HephResult	createDevice(HephDeviceCreateInfo& createInfo);
};
