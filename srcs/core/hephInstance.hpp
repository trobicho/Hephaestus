#pragma once

#include "hephResult.hpp"
#include "../extensions/hephExtensionInterface.hpp"
#include "hephQueueReserveInterface.hpp"
#include <vulkan/vulkan_core.h>
#include <vector>

struct	HephInstanceDebugInfo	{
	bool	debug						= false;
};

struct HephInstanceCreateInfo {
	const char*												pApplicationName;	
	uint32_t													applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	const char*												pEngineName = "";	
	uint32_t													engineVersion = VK_MAKE_VERSION(1, 0, 0);
	uint32_t													apiVersion = VK_API_VERSION_1_3;
	HephInstanceDebugInfo							hephInstanceDebugInfo;
	HephInstanceExtensionInterface**	ppHephInstanceExtensions = nullptr;
	uint32_t													hephInstanceExtensionCount = 0;

	const char**											ppVkInstanceExtensions = nullptr;
	uint32_t													vkInstanceExtensionCount = 0;
	const char**											ppVkValidationLayers = nullptr;
	uint32_t													vkValidationLayerCount = 0;
};

struct HephDeviceCreateInfo {
	HephQueueReserveInterface*				pQueueReserveInterface = nullptr;
	HephDeviceExtensionInterface**		ppHephDeviceExtensions;
	uint32_t													hephDeviceExtensionsCount;

	const char**											ppVkDeviceExtensions = nullptr;
	uint32_t													vkDeviceExtensionCount = 0;
	const char**											ppVkValidationLayers = nullptr;
	uint32_t													vkValidationLayerCount = 0;
	const VkPhysicalDeviceFeatures*		pVkPhysicalDeviceFeatures = nullptr;
	const VkPhysicalDeviceFeatures2*	pVkPhysicalDeviceFeatures2 = nullptr;
};

struct	HephInitializationInfo {
	VkDevice					device;
	VkQueue						queue;
};

struct	HephDevice {
	HephDevice() {};
	HephDevice(const HephDevice &dev, VkQueueFlags flags = 0xFFFFFF) {
		device = dev.device;
		physicalDevice = dev.physicalDevice;
		pAllocationCallbacks = dev.pAllocationCallbacks;
		for (auto& queue : dev.queues) {
			if (queue.flags & flags)
				queues.push_back(queue);
		}
	};

	VkDevice								device;
	std::vector<HephQueue>	queues;
	VkPhysicalDevice				physicalDevice;
	VkAllocationCallbacks*	pAllocationCallbacks = nullptr;
};

class		HephInstanceBase {
	public:
		VkInstance				vulkanInstance;
};

class		HephInstance: public HephInstanceBase {
	public:
		HephInstance() {};
		~HephInstance() {destroy();}

		HephResult	create(HephInstanceCreateInfo& createInfo);
		HephResult	destroy();

		void														addPhysicalDevice(VkPhysicalDevice physicalDevice) {m_physicalDevices.push_back(physicalDevice);}
		std::vector<VkPhysicalDevice>		getPhysicalDevices() {return (m_physicalDevices);}
		std::vector<HephDevice>					getDevices() {return (m_hephDevices);}
		HephResult											createDevice(HephDeviceCreateInfo& createInfo, HephDevice* device = nullptr);

	private:
		std::vector<HephDevice>					m_hephDevices;
		std::vector<VkPhysicalDevice>		m_physicalDevices;
		VkAllocationCallbacks*					m_pAllocationCallbacks = nullptr;
};
