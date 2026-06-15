#pragma once

#include "hephExtensionInterface.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

class	HephExtensionScreenRendering: public HephDeviceExtensionInterface, public HephInstanceExtensionInterface {
	public:
		inline std::vector<const char*> instanceExtensions() override {
			return (std::vector<const char*> {
				VK_KHR_SURFACE_EXTENSION_NAME,
			});
		}
		inline std::vector<const char*> deviceExtensions() override {
			return (std::vector<const char*> {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			});
		}
		void	deviceFunctionLoader(VkDevice device) override;
};
