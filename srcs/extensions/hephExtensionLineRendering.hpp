#pragma once

#include "hephExtensionInterface.hpp"
#include <cstdint>
#include <cstring>
#include <memory>
#include <vulkan/vulkan_core.h>

class	HephExtensionDynamicRendering: public HephDeviceExtensionInterface {
	public:
		inline std::vector<const char*> deviceExtensions() override {
      return (std::vector<const char*>{});
		}
		inline uint32_t                 deviceFeatures(void* data = nullptr, void* link = nullptr) override {
      if (data != nullptr) {
        VkPhysicalDeviceFill dynamicRendering = {
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
          .pNext = link,
          .dynamicRendering = VK_TRUE,
        };
        memcpy(data, &dynamicRendering, sizeof(dynamicRendering));
      }
      return (sizeof(VkPhysicalDeviceDynamicRenderingFeaturesKHR));
		}
		void	deviceFunctionLoader(VkDevice device) override;
};
