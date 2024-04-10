#include <vulkan/vulkan_core.h>
#include "hephQueueReserveInterface.hpp"
#include <iostream>

namespace HephHelper {

inline void	printQueueFamilyInfo(HephQueueFamilyProperties props) {
	VkQueueFlags flags = props.props.queueFamilyProperties.queueFlags;
	std::cout << "queueFamilyFlags: ";
	if (flags & VK_QUEUE_GRAPHICS_BIT)
		std::cout << "VK_QUEUE_GRAPHICS_BIT ";
	if (flags & VK_QUEUE_COMPUTE_BIT)
		std::cout << "VK_QUEUE_COMPUTE_BIT ";
	if (flags & VK_QUEUE_TRANSFER_BIT)
		std::cout << "VK_QUEUE_TRANSFER_BIT ";
	if (flags & VK_QUEUE_SPARSE_BINDING_BIT)
		std::cout << "VK_QUEUE_BINDING_BIT ";
	if (flags & VK_QUEUE_PROTECTED_BIT) // Provided by VK_VERSION_1_1
		std::cout << "VK_QUEUE_BINDING_BIT ";
	if (flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) // Provided by VK_KHR_video_decode_queue
		std::cout << "VK_QUEUE_VIDEO_DECODE_BIT_KHR ";
	if (flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) // Provided by VK_KHR_video_encode_queue
		std::cout << "VK_QUEUE_VIDEO_ENCODE_BIT_KHR ";
	if (flags & VK_QUEUE_OPTICAL_FLOW_BIT_NV) // Provided by VK_NV_optical_flow
		std::cout << "VK_QUEUE_OPTICAL_FLOW_BIT_NV ";
	std::cout << std::endl;
	std::cout << "queueFamilyCount = " << props.props.queueFamilyProperties.queueCount << std::endl;
}

}
