#include "hephExtensionDynamicRendering.hpp"
#include <vulkan/vulkan_core.h>

static PFN_vkCmdBeginRenderingKHR   pfn_vkCmdBeginRenderingKHR = 0;
static PFN_vkCmdEndRenderingKHR     pfn_vkCmdEndRenderingKHR = 0;

void  HephExtensionDynamicRendering::deviceFunctionLoader(VkDevice device) {
  pfn_vkCmdBeginRenderingKHR =
    reinterpret_cast<PFN_vkCmdBeginRenderingKHR>(vkGetDeviceProcAddr(device, "vkCmdBeginRenderingKHR"));
  pfn_vkCmdEndRenderingKHR =
    reinterpret_cast<PFN_vkCmdEndRenderingKHR>(vkGetDeviceProcAddr(device, "vkCmdEndRenderingKHR"));
}
