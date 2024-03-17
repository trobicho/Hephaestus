#include "hephInstance.hpp"
#include "hephResult.hpp"
#include <vulkan/vulkan_core.h>

HephResult	HephInstance::destroy() {
	return (HephResult());
}

HephResult	HephInstance::create(HephInstanceCreateInfo& createInfo) {
	VkApplicationInfo	appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = createInfo.pApplicationName,
		.applicationVersion = createInfo.apiVersion,
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

HephResult	HephInstance::createDevice(HephDeviceCreateInfo& createInfo) {
	return (HephResult());
}
