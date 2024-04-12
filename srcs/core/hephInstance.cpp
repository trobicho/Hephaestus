#include "hephInstance.hpp"
#include "hephResult.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

HephResult	HephInstance::destroy() {
	vkDestroyInstance(vulkanInstance, m_pAllocationCallbacks);
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
	for (int i = 0; i < createInfo.vkInstanceExtensionCount; i++) {
		instanceExtensionNames.push_back(createInfo.ppVkInstanceExtensions[i]);
	}
	for (int i = 0; i < createInfo.vkValidationLayerCount; i++) {
		instanceLayerNames.push_back(createInfo.ppVkValidationLayers[i]);
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
	HEPH_CHECK_RESULT(HephResult("No VkPhysicalDevice in instance.", !m_physicalDevices.empty()));
	HEPH_CHECK_RESULT(HephResult("No HephQueueReserveInterface provided.", createInfo.pQueueReserveInterface != nullptr));
	HephDevice deviceTmp;
	deviceTmp.physicalDevice = m_physicalDevices[0];//multiple physicalDevices
	
	std::vector<const char*>						deviceExtensionNames;
	std::vector<const char*>						deviceLayerNames;
	std::list<std::unique_ptr<void*>>		featuresBuffer;
	for (int i = 0; i < createInfo.hephDeviceExtensionsCount; i++) {
		for (auto extensionName: createInfo.ppHephDeviceExtensions[i]->deviceExtensions()) {
			deviceExtensionNames.push_back(extensionName);
		}
		for (auto layerName: createInfo.ppHephDeviceExtensions[i]->deviceValidationLayers()) {
			deviceLayerNames.push_back(layerName);
		}
		if (featuresBuffer.empty())
			featuresBuffer = createInfo.ppHephDeviceExtensions[i]->deviceFeatures();
		else {
			VkPhysicalDeviceFeatures2KHR* feats = static_cast<VkPhysicalDeviceFeatures2KHR*>(*featuresBuffer.end()->get());
			feats->pNext = *createInfo.ppHephDeviceExtensions[i]->deviceFeatures().front();
		}
	}

	HEPH_CHECK_RESULT(createInfo.pQueueReserveInterface->reserve(deviceTmp.physicalDevice));
	uint32_t												queueCreateInfoCount = 0;
	const VkDeviceQueueCreateInfo*	queueCreateInfos = createInfo.pQueueReserveInterface->getQueueCreateInfos(&queueCreateInfoCount);
	if (queueCreateInfoCount == 0)
		return (HephResult("empty QueueCreateInfo."));

	VkDeviceCreateInfo	deviceInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		//.pNext = featuresBuffer.front().get(),
		.queueCreateInfoCount = queueCreateInfoCount,
		.pQueueCreateInfos = queueCreateInfos,
		.enabledLayerCount= static_cast<uint32_t>(deviceLayerNames.size()),
		.ppEnabledLayerNames= deviceLayerNames.data(),
		.enabledExtensionCount = static_cast<uint32_t>(deviceExtensionNames.size()),
		.ppEnabledExtensionNames = deviceExtensionNames.data(),
	};

	HEPH_CHECK_RESULT(HephResult(vkCreateDevice(deviceTmp.physicalDevice, &deviceInfo, m_pAllocationCallbacks, &deviceTmp.device)
				, "Failed to create Logical Device! ({})"));

	for (int i = 0; i < createInfo.hephDeviceExtensionsCount; i++) {
		createInfo.ppHephDeviceExtensions[i]->deviceFunctionLoader(deviceTmp.device);
	}

	if (device != nullptr)
		*device = deviceTmp;
	m_hephDevices.push_back(deviceTmp);
	return (HephResult());
}
