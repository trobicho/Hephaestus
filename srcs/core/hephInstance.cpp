#include "hephInstance.hpp"
#include "hephResult.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

HephResult	HephInstance::destroy() {
	for (auto& dev: m_hephDevices) {
		vkDestroyDevice(dev.device, dev.pAllocationCallbacks);
		dev.device = VK_NULL_HANDLE;
	}
	vkDestroyInstance(vulkanInstance, m_pAllocationCallbacks);
	vulkanInstance = VK_NULL_HANDLE;
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
	for (int i = 0; i < createInfo.hephInstanceExtensionCount; i++) {
		createInfo.ppHephInstanceExtensions[i]->instanceFunctionLoader(vulkanInstance);
	}
	return (HephResult());
}

HephResult	HephInstance::createDevice(HephDeviceCreateInfo& createInfo, HephDevice* device) {
	HEPH_CHECK_RESULT(HephResult("No VkPhysicalDevice in instance.", !m_physicalDevices.empty()));
	HEPH_CHECK_RESULT(HephResult("No HephQueueReserveInterface provided.", createInfo.pQueueReserveInterface != nullptr));
	HephDevice deviceTmp;
	deviceTmp.physicalDevice = m_physicalDevices[0];//multiple physicalDevices
	
	std::vector<const char*>						deviceExtensionNames;
	std::vector<const char*>						deviceLayerNames;
	void*                               featuresBuffer;
	void*                               featuresBufferOffest;
  void*                               featuresBufferLink = nullptr;
	uint32_t                            featuresBufferSize = 0;
	for (int i = 0; i < createInfo.hephDeviceExtensionsCount; i++) {
		for (auto extensionName: createInfo.ppHephDeviceExtensions[i]->deviceExtensions()) {
			deviceExtensionNames.push_back(extensionName);
		}
		for (auto layerName: createInfo.ppHephDeviceExtensions[i]->deviceValidationLayers()) {
			deviceLayerNames.push_back(layerName);
		}
    featuresBufferSize += createInfo.ppHephDeviceExtensions[i]->deviceFeatures();
	}
  if (featuresBufferSize > 0) {
    featuresBuffer = malloc(featuresBufferSize);
    featuresBufferOffest = featuresBuffer;
    uint32_t  offset = 0;
    for (int i = 0; i < createInfo.hephDeviceExtensionsCount; i++) {
      offset = createInfo.ppHephDeviceExtensions[i]->deviceFeatures(featuresBufferOffest, featuresBufferLink);
      if (offset > 0) {
        featuresBufferLink = featuresBufferOffest;
        featuresBufferOffest = (char*)featuresBufferOffest + offset;
      }
    }
  }

	HEPH_CHECK_RESULT(createInfo.pQueueReserveInterface->reserve(deviceTmp.physicalDevice));
	uint32_t												queueCreateInfoCount = 0;
	const VkDeviceQueueCreateInfo*	queueCreateInfos = createInfo.pQueueReserveInterface->getQueueCreateInfos(&queueCreateInfoCount);
	if (queueCreateInfoCount == 0)
    return (HephResult("empty QueueCreateInfo."));

  VkPhysicalDeviceFeatures2KHR  features = {
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
    .pNext = featuresBufferLink,
    .features = createInfo.features,
  };

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

  if (featuresBufferSize > 0) {
    deviceInfo.pNext = &features;
  }

	HEPH_CHECK_RESULT(HephResult(vkCreateDevice(deviceTmp.physicalDevice, &deviceInfo, m_pAllocationCallbacks, &deviceTmp.device)
				, "Failed to create Logical Device! ({})"));
	for (const auto& queueReserveInfo : createInfo.pQueueReserveInterface->getReserveInfo()) {
		for (const auto& retrieveInfo : queueReserveInfo.retrieveInfos) {
			HephQueue queueTmp;
			queueTmp.queueIndex = retrieveInfo.queueIndex;
			queueTmp.familyIndex = retrieveInfo.familyIndex;
			queueTmp.flags = queueReserveInfo.flags;
			vkGetDeviceQueue(deviceTmp.device, retrieveInfo.familyIndex, retrieveInfo.queueIndex, &queueTmp.queue);
			deviceTmp.queues.push_back(queueTmp);
		}
	}

	for (int i = 0; i < createInfo.hephDeviceExtensionsCount; i++) {
		createInfo.ppHephDeviceExtensions[i]->deviceFunctionLoader(deviceTmp.device);
	}

	if (device != nullptr)
		*device = deviceTmp;
	m_hephDevices.push_back(deviceTmp);
	return (HephResult());
}
