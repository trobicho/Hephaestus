#pragma once

#include "../core/hephaestus_core.hpp"

class	HephPhysicalDevicesSelectorBase {
	public:
		virtual HephResult	selectDevices(HephInstance& hephInstance) = 0;
};

class	HephPhysicalDevicesSelectorTest: public HephPhysicalDevicesSelectorBase {
	public:
		HephResult	selectDevices(HephInstance& hephInstance) {
			uint32_t                      deviceCount;
			std::vector<VkPhysicalDevice> devices;

			vkEnumeratePhysicalDevices(hephInstance.vulkanInstance, &deviceCount, nullptr);
			if (deviceCount == 0)
				return (HephResult("No suported Physical Devices found !!!"));
			devices.resize(deviceCount);
			vkEnumeratePhysicalDevices(hephInstance.vulkanInstance, &deviceCount, devices.data());

			VkPhysicalDeviceProperties  props;
			VkPhysicalDevice            best = devices[0];

			for (auto& dev : devices) {
				vkGetPhysicalDeviceProperties(dev, &props);
				if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					std::cout << props.deviceName << std::endl;
					hephInstance.addPhysicalDevice(dev);
					return (HephResult());
				}
			}
			return (HephResult("No Discrete GPU found"));
		}
};
