#pragma once

#include "hephExtensionInterface.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

class	HephExtensionVideo: public HephDeviceExtensionInterface, public HephInstanceExtensionInterface {
	public:
		inline std::vector<const char*>						deviceExtensions() {
			return (std::vector<const char*> {
				VK_KHR_VIDEO_QUEUE_EXTENSION_NAME,
				VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME,
				VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME,
				VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME,
				VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME,
				VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME,
				VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME,
				VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME,
			});
		}
		inline std::list<std::unique_ptr<void*>>	deviceFeatures() {
			std::list<std::unique_ptr<void*>>	featuresBuffer;
			//VIDEO FEATURES
			return (featuresBuffer);
		}
		void	instanceFunctionLoader(VkInstance instance);
		void	deviceFunctionLoader(VkDevice device);
		void	printDeviceProperties(VkDevice	device);
};
