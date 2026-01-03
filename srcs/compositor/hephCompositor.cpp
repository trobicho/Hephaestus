#include "hephCompositor.hpp"
#include "gui/hephGui.hpp"

static void s_callbackWindowSize(GLFWwindow* window, int width, int height) {
  HephCompositorOutput* outputPtr= static_cast<HephCompositorOutput*>(glfwGetWindowUserPointer(window));
  outputPtr->callbackWindowSize(window, width, height);
}
  
HephCompositorOutput::HephCompositorOutput(const HephCompositorOutputCreateInfo& createInfo)
  : m_hephInstance(createInfo.hephInstance),
  m_device(createInfo.device)
{
  m_outputName = createInfo.name;
  m_fps = createInfo.fps;
  m_width = createInfo.width;
  m_height = createInfo.height;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  m_glfwWindow = glfwCreateWindow(m_width, m_height, m_outputName.c_str(), NULL, NULL);
  glfwGetWindowSize(m_glfwWindow, &m_width, &m_height);
}

void  HephCompositorOutput::callbackWindowSize(GLFWwindow* window, int width, int height) {
	vkDeviceWaitIdle(m_device.device);
	HEPH_PRINT_RESULT(HephResult(vkQueueWaitIdle(m_device.queues[0].queue), "Error waiting for queue {{}}!"));
  m_width = width;
  m_height = height;

	m_swapchain.destroy();
  vkDestroySurfaceKHR(m_hephInstance.vulkanInstance, m_surface, m_device.pAllocationCallbacks);
  HEPH_PRINT_RESULT(HephResult(glfwCreateWindowSurface(m_hephInstance.vulkanInstance, m_glfwWindow
        , m_device.pAllocationCallbacks, &m_surface), "Failed to create Surface {{}} !"));
  m_swapchain.recreate(VkExtent2D{static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}, m_surface);

  HephGui::setDisplaySize(m_width, m_height);
}

HephResult  HephCompositorOutput::create() {
  HEPH_CHECK_RESULT(HephResult(glfwCreateWindowSurface(m_hephInstance.vulkanInstance, m_glfwWindow
        , m_device.pAllocationCallbacks, &m_surface), "Failed to create Surface {{}} !"));
  HephCommandPoolCreateInfo commandPoolCreateInfo = {
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = m_device.queues[0].familyIndex,
	};
  HEPH_CHECK_RESULT(m_commandPool.create(m_device, commandPoolCreateInfo).errorFormat("Failed to create CommandPool {{}} !"));
	HEPH_CHECK_RESULT(m_allocator.create(m_device).errorFormat("Failed to create Memory Allocator {{}} !"));
	HEPH_CHECK_RESULT(createSwapchain());
	HEPH_CHECK_RESULT(HephResult("Empty swapchain!", (m_swapchain.getImageCount() > 0)));
	m_commandBuffers.resize(m_swapchain.getImageCount());
	HEPH_CHECK_RESULT(m_commandPool.allocate(m_commandBuffers.size(), m_commandBuffers.data()));

	return (HephResult());
}

HephResult	HephCompositorOutput::createSwapchain() {
	VkSurfaceFormatKHR	format = (VkSurfaceFormatKHR){VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	HephSurfaceSupportDetails	surfaceSupportDetails(m_device, m_surface);
	if (surfaceSupportDetails.formats[0].format != VK_FORMAT_UNDEFINED)
    format = surfaceSupportDetails.formats[0];
  format = (VkSurfaceFormatKHR){VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  m_surfaceFormat = format;
  uint32_t minImageCount = std::min(3u, surfaceSupportDetails.capabilies.minImageCount);
  if (3 < surfaceSupportDetails.capabilies.maxImageCount && 3 > minImageCount)
    minImageCount = 3;
	//HEPH_CHECK_RESULT(createRenderPass());
	HephSwapchainCreateInfo		swapchainCreateInfo;
	VkSwapchainCreateInfoKHR	swapInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = m_surface,
		.minImageCount = minImageCount,
		.imageFormat = format.format,
		.imageColorSpace = format.colorSpace,
		.imageExtent = VkExtent2D{.width = static_cast<uint32_t>(m_width), .height=static_cast<uint32_t>(m_height)},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr,
		.preTransform = surfaceSupportDetails.capabilies.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE,
	};
	swapchainCreateInfo.swapchainCreateInfo = swapInfo;
	swapchainCreateInfo.renderPass = m_renderPass;
	HEPH_CHECK_RESULT(m_swapchain.create(m_device, swapchainCreateInfo));
	return (HephResult());
}

HephResult	HephCompositor::instanceSetup() {
	uint32_t glfwExtCount = 1;
	const char **glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
  std::vector<const char*> vkExts;
  for (int i = 0; i < glfwExtCount; i++)
    vkExts.push_back(glfwExts[i]);

	std::vector<HephInstanceExtensionInterface*>	instanceExtensions;
	HephExtensionDebug                            extDebug;
	HephExtensionScreenRendering									extScreenRendering;
	HephExtensionDynamicRendering                 extDynamicRendering;
	instanceExtensions.push_back(&extDebug);
	instanceExtensions.push_back(&extScreenRendering);

	HephInstanceCreateInfo		instanceCreateInfo = {
		.pApplicationName = "Heph Compositor",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = "Heph Compositor",
		.engineVersion = VK_MAKE_VERSION(0, 1, 0),
		.hephInstanceDebugInfo = {
			.debug = true,
		},
		.ppHephInstanceExtensions = instanceExtensions.data(),
		.hephInstanceExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
		.ppVkInstanceExtensions =  vkExts.data(),
		.vkInstanceExtensionCount = (uint32_t)vkExts.size(),
	};
	HEPH_CHECK_RESULT(m_hephInstance.create(instanceCreateInfo));

	HephPhysicalDevicesSelectorTest	devicesSelect;
	devicesSelect.selectDevices(m_hephInstance);
	std::vector<HephDeviceExtensionInterface*>	deviceExtensions;
	deviceExtensions.push_back(&extScreenRendering);
  deviceExtensions.push_back(&extDynamicRendering);

	std::vector<HephQueueReserveInfo>	queueReserveInfos;
	queueReserveInfos.push_back((HephQueueReserveInfo) {
		.flags = VK_QUEUE_GRAPHICS_BIT,
		.priority = 1.0,
		.count = 1,
	});
	queueReserveInfos.push_back((HephQueueReserveInfo) {
		.flags = VK_QUEUE_TRANSFER_BIT,
		.priority = 1.0,
		.count = 0,
	});
	HephQueueReserveBasic			queueReserveBasic;
	queueReserveBasic.addReserveInfo(queueReserveInfos);
	HephDeviceCreateInfo			deviceCreateInfo = {
		.pQueueReserveInterface = static_cast<HephQueueReserveInterface*>(&queueReserveBasic),
		.ppHephDeviceExtensions = deviceExtensions.data(),
		.hephDeviceExtensionsCount = static_cast<uint32_t>(deviceExtensions.size()),
	};
	HEPH_CHECK_RESULT(m_hephInstance.createDevice(deviceCreateInfo, &m_device));

	return (HephResult());
}
