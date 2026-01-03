#include "hephExtensionScreenRendering.hpp"

#include "hephExtensionScreenRendering.hpp"
#include <vulkan/vulkan_core.h>

static PFN_vkAcquireNextImageKHR										pfn_vkAcquireNextImageKHR = 0;
static PFN_vkCreateSwapchainKHR											pfn_vkCreateSwapchainKHR = 0;
static PFN_vkDestroySwapchainKHR										pfn_vkDestroySwapchainKHR = 0;
static PFN_vkGetSwapchainImagesKHR									pfn_vkGetSwapchainImagesKHR = 0;
static PFN_vkQueuePresentKHR												pfn_vkQueuePresentKHR = 0;
//VULKAN VERSION >= 1.1
static PFN_vkAcquireNextImage2KHR										pfn_vkAcquireNextImage2KHR = 0;
static PFN_vkGetDeviceGroupPresentCapabilitiesKHR		pfn_vkGetDeviceGroupPresentCapabilitiesKHR = 0;
static PFN_vkGetDeviceGroupSurfacePresentModesKHR		pfn_vkGetDeviceGroupSurfacePresentModesKHR = 0;
static PFN_vkGetPhysicalDevicePresentRectanglesKHR	pfn_vkGetPhysicalDevicePresentRectanglesKHR = 0;

void  HephExtensionScreenRendering::deviceFunctionLoader(VkDevice device) {
	pfn_vkCreateSwapchainKHR =
    reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR"));
	pfn_vkAcquireNextImageKHR =
    reinterpret_cast<PFN_vkAcquireNextImageKHR>(vkGetDeviceProcAddr(device, "vkAcquireNextImageKHR"));
	pfn_vkDestroySwapchainKHR =
    reinterpret_cast<PFN_vkDestroySwapchainKHR>(vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR"));
	pfn_vkGetSwapchainImagesKHR =
    reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR"));
	pfn_vkQueuePresentKHR =
    reinterpret_cast<PFN_vkQueuePresentKHR>(vkGetDeviceProcAddr(device, "vkQueuePresentKHR"));
	pfn_vkAcquireNextImage2KHR =
    reinterpret_cast<PFN_vkAcquireNextImage2KHR>(vkGetDeviceProcAddr(device, "vkAcquireNextImage2KHR"));
	pfn_vkGetDeviceGroupPresentCapabilitiesKHR =
    reinterpret_cast<PFN_vkGetDeviceGroupPresentCapabilitiesKHR>(vkGetDeviceProcAddr(device, "vkGetDeviceGroupPresentCapabilitiesKHR"));
	pfn_vkGetDeviceGroupSurfacePresentModesKHR =
    reinterpret_cast<PFN_vkGetDeviceGroupSurfacePresentModesKHR>(vkGetDeviceProcAddr(device, "vkGetDeviceGroupSurfacePresentModesKHR"));
	pfn_vkGetPhysicalDevicePresentRectanglesKHR =
    reinterpret_cast<PFN_vkGetPhysicalDevicePresentRectanglesKHR>(vkGetDeviceProcAddr(device, "vkGetPhysicalDevicePresentRectanglesKHR"));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkCreateSwapchainKHR(VkDevice device
																	, const VkSwapchainCreateInfoKHR *pCreateInfo
																	, const VkAllocationCallbacks *pAllocator
																	, VkSwapchainKHR *pSwapchain)
{
	return (pfn_vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain));
}


VKAPI_ATTR VkResult VKAPI_CALL	vkAcquireNextImageKHR(VkDevice device
																	, VkSwapchainKHR swapchain
																	, uint64_t timeout
																	, VkSemaphore semaphore
																	, VkFence fence
																	, uint32_t *pImageIndex)
{
	return (pfn_vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex));
}
VKAPI_ATTR void VKAPI_CALL			vkDestroySwapchainKHR(VkDevice device
																	, VkSwapchainKHR swapchain
																	, const VkAllocationCallbacks *pAllocator)
{
	pfn_vkDestroySwapchainKHR(device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL	vkGetSwapchainImagesKHR(VkDevice device
																	, VkSwapchainKHR swapchain
																	, uint32_t *pSwapchainImageCount
																	, VkImage *pSwapchainImages)
{
	return (pfn_vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo)
{
	return (pfn_vkQueuePresentKHR(queue, pPresentInfo));
}

//VULKAN VERSION >= 1.1
VKAPI_ATTR VkResult VKAPI_CALL	vkAcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR *pAcquireInfo, uint32_t *pImageIndex)
{
	return (pfn_vkAcquireNextImage2KHR(device, pAcquireInfo, pImageIndex));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkGetDeviceGroupPresentCapabilitiesKHR(VkDevice device
																	, VkDeviceGroupPresentCapabilitiesKHR *pDeviceGroupPresentCapabilities)
{
	return (pfn_vkGetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities));
}
VKAPI_ATTR VkResult VKAPI_CALL	vkGetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR *pModes)
{
	return (vkGetDeviceGroupSurfacePresentModesKHR(device, surface, pModes));
}
VKAPI_ATTR VkResult VKAPI_CALL	vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice
																	, VkSurfaceKHR surface
																	, uint32_t *pRectCount
																	, VkRect2D *pRects)
{
	return (pfn_vkGetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects));
}
