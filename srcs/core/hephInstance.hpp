#pragma once
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
	const char**									ppInstanceExtensions;
	const uint32_t								instanceExtensionCount;
};

struct HephDeviceCreateInfo {
	const char**									ppDeviceExtensions;
	const uint32_t								deviceExtensionCount;
	const char**									ppValidationLayers;
	const uint32_t								validationLayerCount;
	const HephQueueReserveInfo*		pQueueReserveInfos;
	const uint32_t								queueReserveInfoCount;
};

struct	HephInitializationInfo {
	VkDevice					device;
	VkQueue						queue;
};

class		HephInstance: public HephInstanceBase {
	public:
		HephInstance() {};

		HephResult	create(HephInstanceCreateInfo& createInfo) {};
		HephResult	createDevice(HephDeviceCreateInfo& createInfo) {};
};
