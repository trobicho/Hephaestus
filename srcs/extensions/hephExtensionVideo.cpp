#include "hephExtensionVideo.hpp"
#include <vulkan/vulkan_core.h>

static PFN_vkCmdDecodeVideoKHR			pfn_vkCmdDecodeVideoKHR	= 0;

static PFN_vkCmdEncodeVideoKHR			pfn_vkCmdEncodeVideoKHR = 0;
static PFN_vkGetEncodedVideoSessionParametersKHR	pfn_vkGetEncodedVideoSessionParametersKHR = 0;
static PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR pfn_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = 0;

static PFN_vkBindVideoSessionMemoryKHR	pfn_vkBindVideoSessionMemoryKHR = 0;
static PFN_vkCmdBeginVideoCodingKHR	pfn_vkCmdBeginVideoCodingKHR = 0;
static PFN_vkCmdControlVideoCodingKHR	pfn_vkCmdControlVideoCodingKHR = 0;
static PFN_vkCmdEndVideoCodingKHR	pfn_vkCmdEndVideoCodingKHR = 0;
static PFN_vkCreateVideoSessionKHR	pfn_vkCreateVideoSessionKHR = 0;
static PFN_vkCreateVideoSessionParametersKHR	pfn_vkCreateVideoSessionParametersKHR = 0;
static PFN_vkDestroyVideoSessionKHR	pfn_vkDestroyVideoSessionKHR = 0;
static PFN_vkDestroyVideoSessionParametersKHR	pfn_vkDestroyVideoSessionParametersKHR = 0;
static PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR	pfn_vkGetPhysicalDeviceVideoCapabilitiesKHR = 0;
static PFN_vkUpdateVideoSessionParametersKHR	pfn_vkUpdateVideoSessionParametersKHR = 0;
static PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR	pfn_vkGetPhysicalDeviceVideoFormatPropertiesKHR = 0;
static PFN_vkGetVideoSessionMemoryRequirementsKHR	pfn_vkGetVideoSessionMemoryRequirementsKHR = 0;


void  HephExtensionVideo::deviceFunctionLoader(VkDevice device) {
  pfn_vkCmdDecodeVideoKHR =
    reinterpret_cast<PFN_vkCmdDecodeVideoKHR>(vkGetDeviceProcAddr(device, "vkCmdDecodeVideoKHR"));
	pfn_vkCmdEncodeVideoKHR =
    reinterpret_cast<PFN_vkCmdEncodeVideoKHR>(vkGetDeviceProcAddr(device, "vkCmdEncodeVideoKHR"));
	pfn_vkGetEncodedVideoSessionParametersKHR =
		reinterpret_cast<PFN_vkGetEncodedVideoSessionParametersKHR>(vkGetDeviceProcAddr(device, "vkGetEncodedVideoSessionParametersKHR"));
	pfn_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR =
    reinterpret_cast<PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR>(vkGetDeviceProcAddr(device, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR "));

	pfn_vkBindVideoSessionMemoryKHR =
		reinterpret_cast<PFN_vkBindVideoSessionMemoryKHR>(vkGetDeviceProcAddr(device, "vkBindVideoSessionMemoryKHR"));
	pfn_vkCmdBeginVideoCodingKHR =
		reinterpret_cast<PFN_vkCmdBeginVideoCodingKHR>(vkGetDeviceProcAddr(device, "vkCmdBeginVideoCodingKHR"));
	pfn_vkCmdControlVideoCodingKHR =
		reinterpret_cast<PFN_vkCmdControlVideoCodingKHR>(vkGetDeviceProcAddr(device, "vkCmdControlVideoCodingKHR"));
	pfn_vkCmdEndVideoCodingKHR =
		reinterpret_cast<PFN_vkCmdEndVideoCodingKHR>(vkGetDeviceProcAddr(device, "vkCmdEndVideoCodingKHR"));
	pfn_vkCreateVideoSessionKHR =
		reinterpret_cast<PFN_vkCreateVideoSessionKHR>(vkGetDeviceProcAddr(device, "vkCreateVideoSessionKHR"));
	pfn_vkCreateVideoSessionParametersKHR =
		reinterpret_cast<PFN_vkCreateVideoSessionParametersKHR>(vkGetDeviceProcAddr(device, "vkCreateVideoSessionParametersKHR"));
	pfn_vkDestroyVideoSessionKHR =
		reinterpret_cast<PFN_vkDestroyVideoSessionKHR>(vkGetDeviceProcAddr(device, "vkDestroyVideoSessionKHR"));
	pfn_vkDestroyVideoSessionParametersKHR =
		reinterpret_cast<PFN_vkDestroyVideoSessionParametersKHR>(vkGetDeviceProcAddr(device, "vkDestroyVideoSessionParametersKHR"));
	pfn_vkGetPhysicalDeviceVideoCapabilitiesKHR =
		reinterpret_cast<PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR>(vkGetDeviceProcAddr(device, "vkGetPhysicalDeviceVideoCapabilitiesKHR"));
	pfn_vkGetPhysicalDeviceVideoFormatPropertiesKHR =
		reinterpret_cast<PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR>(vkGetDeviceProcAddr(device, "vkGetPhysicalDeviceVideoFormatPropertiesKHR"));
	pfn_vkGetVideoSessionMemoryRequirementsKHR =
		reinterpret_cast<PFN_vkGetVideoSessionMemoryRequirementsKHR>(vkGetDeviceProcAddr(device, "vkGetVideoSessionMemoryRequirementsKHR"));
	pfn_vkUpdateVideoSessionParametersKHR =
		reinterpret_cast<PFN_vkUpdateVideoSessionParametersKHR>(vkGetDeviceProcAddr(device, "vkUpdateVideoSessionParametersKHR"));
}

VKAPI_ATTR void VKAPI_CALL			vkCmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR *pDecodeInfo)
{
  pfn_vkCmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
}

VKAPI_ATTR void VKAPI_CALL			vkCmdEncodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR *pEncodeInfo)
{
  pfn_vkCmdEncodeVideoKHR(commandBuffer, pEncodeInfo);
}

VKAPI_ATTR VkResult VKAPI_CALL	vkGetEncodedVideoSessionParametersKHR(VkDevice device
																	, const VkVideoEncodeSessionParametersGetInfoKHR *pVideoSessionParametersInfo
																	, VkVideoEncodeSessionParametersFeedbackInfoKHR *pFeedbackInfo
																	, size_t *pDataSize
																	, void *pData)
{
	return (pfn_vkGetEncodedVideoSessionParametersKHR(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice
																	, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR *pQualityLevelInfo
																	, VkVideoEncodeQualityLevelPropertiesKHR *pQualityLevelProperties)
{
	return (pfn_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(physicalDevice, pQualityLevelInfo, pQualityLevelProperties));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkBindVideoSessionMemoryKHR(VkDevice device
																	, VkVideoSessionKHR videoSession
																	, uint32_t bindSessionMemoryInfoCount
																	, const VkBindVideoSessionMemoryInfoKHR *pBindSessionMemoryInfos)
{
	return (pfn_vkBindVideoSessionMemoryKHR(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos));
}

VKAPI_ATTR void VKAPI_CALL			vkCmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR *pBeginInfo)
{
	pfn_vkCmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
}

VKAPI_ATTR void VKAPI_CALL			vkCmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR *pCodingControlInfo)
{
	pfn_vkCmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
}

VKAPI_ATTR void VKAPI_CALL			vkCmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR *pEndCodingInfo)
{
	pfn_vkCmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
}

VKAPI_ATTR VkResult VKAPI_CALL	vkCreateVideoSessionKHR(VkDevice device
																	, const VkVideoSessionCreateInfoKHR *pCreateInfo
																	, const VkAllocationCallbacks *pAllocator
																	, VkVideoSessionKHR *pVideoSession)
{
	return(pfn_vkCreateVideoSessionKHR(device, pCreateInfo, pAllocator, pVideoSession));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkCreateVideoSessionParametersKHR(VkDevice device
																	, const VkVideoSessionParametersCreateInfoKHR *pCreateInfo
																	, const VkAllocationCallbacks *pAllocator
																	, VkVideoSessionParametersKHR *pVideoSessionParameters)
{
	return (pfn_vkCreateVideoSessionParametersKHR(device, pCreateInfo, pAllocator, pVideoSessionParameters));
}

VKAPI_ATTR void VKAPI_CALL			vkDestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks *pAllocator)
{
	pfn_vkDestroyVideoSessionKHR(device, videoSession, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL			vkDestroyVideoSessionParametersKHR(VkDevice device
																	, VkVideoSessionParametersKHR videoSessionParameters
																	, const VkAllocationCallbacks *pAllocator)
{
	pfn_vkDestroyVideoSessionParametersKHR(device, videoSessionParameters, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL	vkGetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice
																	, const VkVideoProfileInfoKHR *pVideoProfile
																	, VkVideoCapabilitiesKHR *pCapabilities)
{
	return (pfn_vkGetPhysicalDeviceVideoCapabilitiesKHR(physicalDevice, pVideoProfile, pCapabilities));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkGetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice
																	, const VkPhysicalDeviceVideoFormatInfoKHR *pVideoFormatInfo
																	, uint32_t *pVideoFormatPropertyCount
																	, VkVideoFormatPropertiesKHR *pVideoFormatProperties)
{
	return (pfn_vkGetPhysicalDeviceVideoFormatPropertiesKHR(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkUpdateVideoSessionParametersKHR(VkDevice device
																	, VkVideoSessionParametersKHR videoSessionParameters
																	, const VkVideoSessionParametersUpdateInfoKHR *pUpdateInfo)
{
	return (vkUpdateVideoSessionParametersKHR(device, videoSessionParameters, pUpdateInfo));
}

VKAPI_ATTR VkResult VKAPI_CALL	vkGetVideoSessionMemoryRequirementsKHR(VkDevice device
																	, VkVideoSessionKHR videoSession
																	, uint32_t *pMemoryRequirementsCount
																	, VkVideoSessionMemoryRequirementsKHR *pMemoryRequirements)
{
	return (pfn_vkGetVideoSessionMemoryRequirementsKHR(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements));
}

