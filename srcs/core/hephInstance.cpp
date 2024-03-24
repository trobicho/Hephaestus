#include "hephInstance.hpp"
#include "hephResult.hpp"
#include <algorithm>
#include <cstdint>
#include <vulkan/vulkan_core.h>

static void		queueReservation(std::vector<VkQueueFamilyProperties2> queueFamilyProps
		, std::vector<HephQueueReserveInfo*>& queueReserveInfos
		, std::vector<VkDeviceQueueCreateInfo>& queueCreateInfo)
{
	for (auto& reserveInfo: queueReserveInfos) {
		for (uint32_t i = 0; i < queueFamilyProps.size(); i++) {
			if (queueFamilyProps[i].queueFamilyProperties.queueFlags == reserveInfo->flags && queueFamilyProps[i].queueFamilyProperties.queueCount > 0) {
				uint32_t	queueCount = std::min(queueFamilyProps[i].queueFamilyProperties.queueCount, reserveInfo->count);
				if (queueCount == 0)
					queueCount = queueFamilyProps[i].queueFamilyProperties.queueCount;
				std::vector<float>	priorities;
				priorities.resize(queueCount, reserveInfo->priority);
				queueCreateInfo.push_back((VkDeviceQueueCreateInfo){
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.queueFamilyIndex = i,
					.queueCount = queueCount,
					.pQueuePriorities = priorities.data(),
				});
				queueFamilyProps[i].queueFamilyProperties.queueCount -= queueCount;
			}
		}
	}
}

HephResult	HephInstance::destroy() {
	return (HephResult());
}

HephResult	HephInstance::create(HephInstanceCreateInfo& createInfo) {
	VkApplicationInfo	appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = createInfo.pApplicationName,
		.applicationVersion = createInfo.applicationVersion,
		.pEngineName = createInfo.pEngineName,
		.engineVersion = createInfo.engineVersion,
		.apiVersion = createInfo.apiVersion,
	};

	std::vector<const char*>	instanceExtensionNames;
	std::vector<const char*>	instanceLayerNames;
	for (int i = 0; i < createInfo.hephInstanceExtensionCount; i++) {
		for (auto extensionName: createInfo.ppHephInstanceExtensions[i]->instanceExtensions()) {
			instanceExtensionNames.push_back(extensionName);
		}
		for (auto layerName: createInfo.ppHephInstanceExtensions[i]->instanceValidationLayers()) {
			instanceLayerNames.push_back(layerName);
		}
	}

	VkInstanceCreateInfo	instanceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = static_cast<uint32_t>(instanceLayerNames.size()),
		.ppEnabledLayerNames = instanceLayerNames.data(),
		.enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNames.size()),
		.ppEnabledExtensionNames = instanceExtensionNames.data(),
	};
	HEPH_CHECK_RESULT(HephResult(vkCreateInstance(&instanceCreateInfo, m_pAllocationCallbacks, &vulkanInstance)
				, "Error in creating the Vulkan Instance: {} !!!"));
	return (HephResult());
}

HephResult	HephInstance::createDevice(HephDeviceCreateInfo& createInfo, HephDevice* device) {
	if (m_physicalDevices.empty())
		return (HephResult("No VkPhysicalDevice in instance"));
	HephDevice deviceTmp;
	deviceTmp.physicalDevice = m_physicalDevices[0];//multiple physicalDevices


	std::vector<VkQueueFamilyProperties2>										queueFamilyProps;
	std::vector<VkQueueFamilyGlobalPriorityPropertiesKHR>		queueFamilyGlobalPriorityProps;
	uint32_t																								queueCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties2(deviceTmp.physicalDevice, &queueCount, nullptr);
	if (queueCount == 0)
		return (HephResult("No Queue Found !"));
	queueFamilyProps.resize(queueCount);
	queueFamilyGlobalPriorityProps.resize(queueCount);
	for (int i = 0; i < queueCount; i++) {
		queueFamilyProps[i].pNext = &queueFamilyGlobalPriorityProps[i];
	}
	vkGetPhysicalDeviceQueueFamilyProperties2(deviceTmp.physicalDevice, &queueCount, queueFamilyProps.data());

	if (device != nullptr)
		*device = deviceTmp;
	return (HephResult());
}
