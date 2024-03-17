#pragma once

#include "hephResult.hpp"
#include "../extensions/hephExtensionInterface.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <vector>

struct	HephInstanceDebugInfo	{
	bool	debug						= false;
};

struct	HephQueue {
	VkQueue				queue;
	VkQueueFlags	flags;
	bool					inUse = false;
};

struct	HephQueueReserveInfo {
	VkQueueFlags	flags;
	uint32_t			reserveCount;
};

struct HephInstanceCreateInfo {
	const char*												pApplicationName;	
	const char*												pEngineName = "";	
	uint32_t													engineVersion = VK_MAKE_VERSION(1, 0, 0);
	uint32_t													apiVersion = VK_API_VERSION_1_3;
	HephInstanceDebugInfo							hephInstanceDebugInfo;
	HephInstanceExtensionInterface**	ppHephInstanceExtensions;
	uint32_t													hephInstanceExtensionCount;

	const char**											ppVkInstanceExtensions;
	uint32_t													kvkInstanceExtensionCount;
	const char**											ppVkValidationLayers;
	uint32_t													vkValidationLayerCount;
};

struct HephDeviceCreateInfo {
	const HephQueueReserveInfo*				pQueueReserveInfos;
	uint32_t													queueReserveInfoCount;
	HephDeviceExtensionInterface*			hephDeviceExtensionsInfo;

	const char**											ppVkDeviceExtensions;
	uint32_t													vkDeviceExtensionCount;
	const char**											ppVkValidationLayers;
	uint32_t													vkValidationLayerCount;
	const VkPhysicalDeviceFeatures*		pVkPhysicalDeviceFeatures;
	const VkPhysicalDeviceFeatures2*	pVkPhysicalDeviceFeatures2;
};

struct	HephInitializationInfo {
	VkDevice					device;
	VkQueue						queue;
};

struct	HephDevice {
	VkDevice								device;
	std::vector<HephQueue>	queues;
	VkPhysicalDevice				physicalDevice;
};

class		HephInstanceBase {
	public:
		VkInstance				vulkanInstance;
		HephDevice				device;
		VkPhysicalDevice	physicalDevice;
};

class		HephInstance: public HephInstanceBase {
	public:
		HephInstance() {};

		HephResult	create(HephInstanceCreateInfo& createInfo);
		HephResult	destroy();

		HephResult	createDevice(HephDeviceCreateInfo& createInfo);

	private:
		std::vector<HephDevice>					m_hephDevices;
		std::vector<VkPhysicalDevice>		m_physicalDevices;
		VkAllocationCallbacks*					m_pAllocationCallbacks = nullptr;
};
