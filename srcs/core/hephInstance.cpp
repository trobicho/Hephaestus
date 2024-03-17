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

	std::vector<const char*>	instanceLayerNames;
	std::vector<const char*>	instanceExtensionNames;

	VkInstanceCreateInfo	instanceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = static_cast<uint32_t>(instanceLayerNames.size()),
		.ppEnabledLayerNames = instanceLayerNames.data(),
		.enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNames.size()),
		.ppEnabledExtensionNames = instanceExtensionNames.data(),
	};
	HEPH_CHECK_RESULT(HephResult(vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance), "Error in creating the Vulkan Instance"));
	return (HephResult());
}

HephResult	HephInstance::createDevice(HephDeviceCreateInfo& createInfo) {
	return (HephResult());
}
