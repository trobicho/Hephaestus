#pragma once

#include "hephExtensionInterface.hpp"
#include <vulkan/vulkan_core.h>

class	HephExtensionDebug: public HephInstanceExtensionInterface {
	public:
		inline std::vector<const char*>						instanceExtensions() {
			return (std::vector<const char*> {
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
			});
		}
		inline std::vector<const char*>						instanceValidationLayers() {
			return (std::vector<const char*> {
				"VK_LAYER_KHRONOS_validation",
			});
		}
};
