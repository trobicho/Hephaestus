#pragma once

#include "hephExtensionInterface.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

class	HephExtensionScreenRendering: public HephDeviceExtensionInterface, public HephInstanceExtensionInterface {
	public:
		inline std::vector<const char*>						instanceExtensions() {
			return (std::vector<const char*> {
				VK_KHR_SURFACE_EXTENSION_NAME,
			});
		}
		inline std::vector<const char*>						deviceExtensions() {
			return (std::vector<const char*> {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			});
		}
		inline std::list<std::unique_ptr<void*>>	deviceFeatures() {
			std::list<std::unique_ptr<void*>>	featuresBuffer;
			//VIDEO FEATURES
			return (featuresBuffer);
		}
		void	deviceFunctionLoader(VkDevice device);
};
