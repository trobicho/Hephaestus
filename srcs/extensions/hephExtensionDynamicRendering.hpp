#pragma once

#include "hephExtensionInterface.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

class	HephExtensionDynamicRendering: public HephDeviceExtensionInterface {
	public:
		inline std::vector<const char*>						deviceExtensions() override {
			return (std::vector<const char*> {
				VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
			});
		}
		inline std::list<std::unique_ptr<void*>>	deviceFeatures() override {
			std::list<std::unique_ptr<void*>>	featuresBuffer;
			//VIDEO FEATURES
			return (featuresBuffer);
		}
		void	deviceFunctionLoader(VkDevice device) override;
};
