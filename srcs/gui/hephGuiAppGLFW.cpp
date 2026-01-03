#include "hephGuiAppGLFW.hpp"
#include "hephGuiIoGLFW.hpp"
#include "thread"
#include <cstdint>

static void s_callbackWindowSize(GLFWwindow* window, int width, int height) {
  HephGuiAppGLFW *userPtr = static_cast<HephGuiAppGLFW*>(glfwGetWindowUserPointer(window));
  userPtr->callbackWindowSize(window, width, height);
}

void  HephGuiAppGLFW::callbackWindowSize(GLFWwindow* window, int width, int height) {
	HEPH_PRINT_RESULT(HephResult(vkQueueWaitIdle(m_device.queues[0].queue), "Error waiting for queue {{}}!"));
  m_width = width;
  m_height = height;

	m_swapchain.destroy();
  vkDestroySurfaceKHR(m_hephInstance.vulkanInstance, m_surface, m_device.pAllocationCallbacks);
  HEPH_PRINT_RESULT(HephResult(glfwCreateWindowSurface(m_hephInstance.vulkanInstance, m_mainWindow
        , m_device.pAllocationCallbacks, &m_surface), "Failed to create Surface {{}} !"));
  m_swapchain.recreate(VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)}, m_surface);

  HephGui::setDisplaySize(m_width, m_height);
  updateLayout();
}

HephResult	HephGuiAppGLFW::create(const HephGuiAppGLFWCreateInfo& createInfo) {
  glfwInit();
  m_framerate = createInfo.framerate;
	HEPH_CHECK_RESULT(hephaestusSetup(createInfo));
  HEPH_CHECK_RESULT(HephGui::init());

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  m_mainWindow = glfwCreateWindow(createInfo.windowWidth, createInfo.windowHeight, createInfo.pApplicationName, NULL, NULL);
  glfwGetWindowSize(m_mainWindow, &m_width, &m_height);
  setupCallbackForWindow(m_mainWindow);

  HEPH_CHECK_RESULT(HephResult(glfwCreateWindowSurface(m_hephInstance.vulkanInstance, m_mainWindow
        , m_device.pAllocationCallbacks, &m_surface), "Failed to create Surface {{}} !"));
	HephCommandPoolCreateInfo commandPoolCreateInfo = {
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = m_device.queues[0].familyIndex,
	};
	HEPH_CHECK_RESULT(m_commandPool.create(m_device, commandPoolCreateInfo).errorFormat("Failed to create CommandPool {{}} !"));
	HEPH_CHECK_RESULT(m_allocator.create(m_device).errorFormat("Failed to create Memory Allocator {{}} !"));
	HEPH_CHECK_RESULT(createHephSwapchain());
	HEPH_CHECK_RESULT(HephResult("Empty swapchain!", (m_swapchain.getImageCount() > 0)));
	m_commandBuffers.resize(m_swapchain.getImageCount());
	HEPH_CHECK_RESULT(m_commandPool.allocate(m_commandBuffers.size(), m_commandBuffers.data()));

  HephGui::setDisplaySize(m_width, m_height);
  HephGui::create(m_device, m_renderPass);

	return (HephResult());
}

void  HephGuiAppGLFW::destroy() {
  std::cout << "DESTROY" << std::endl;

  HephGLFW_destroy();
	vkDeviceWaitIdle(m_device.device);
  {
    m_swapchain.destroy();
    vkDestroySurfaceKHR(m_hephInstance.vulkanInstance, m_surface, m_device.pAllocationCallbacks);
    vkDestroyRenderPass(m_device.device, m_renderPass, m_device.pAllocationCallbacks);
    m_commandPool.destroy();
  }
  glfwDestroyWindow(m_mainWindow);
  glfwTerminate();
  std::cout << "Everything was successfully destroyed" << std::endl;
}

void  HephGuiAppGLFW::run() {
	double lastUpdateTime = 0;

  while(!glfwWindowShouldClose(m_mainWindow) && !m_quit) {
		double now = glfwGetTime();
		double deltaTime = now - lastUpdateTime;
    glfwPollEvents();
    HEPH_PRINT_RESULT(render());
		if (deltaTime < m_framerate) {
			//std::cout << "expect: " <<  m_framerate << "delta: " << deltaTime << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<uint32_t>(1000.0 * (m_framerate - deltaTime))));
		}
		lastUpdateTime = now;
  }
}

HephResult	HephGuiAppGLFW::render() {
	HephSwapchainPresentData	presentData;

	HephResult	result = m_swapchain.acquireNextImage(presentData);
  if (result.vkResult == VK_ERROR_OUT_OF_DATE_KHR)
    return (HephResult());
  if (result.vkResult != VK_SUBOPTIMAL_KHR && !result.valid())
    return (HephResult(result.vkResult, "problem acquiring next frame ({}) !!"));

  auto& commandBuffer = m_commandBuffers[presentData.imageCurrent];
  vkResetCommandBuffer(commandBuffer, 0);
  VkCommandBufferBeginInfo  beginInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    .pInheritanceInfo = nullptr,
  };

  HephGui::NewFrame();
  renderGui();
  HephGLFW_render(m_mainWindow);

  vkBeginCommandBuffer(commandBuffer, &beginInfo);
  {
    VkClearValue clearValue = (VkClearValue){0.1f, 0.1f, 0.1f, 1.0f};
    VkRenderPassBeginInfo     renderPassInfo = {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass = m_renderPass,
      .framebuffer = presentData.image.framebuffer,
      .renderArea = (VkRect2D) {
        .offset = (VkOffset2D){0, 0},
        .extent = presentData.extent,
      },
      .clearValueCount = 1,
      .pClearValues = &clearValue,
    };
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    VkRect2D		scissor = {
      .offset = {0, 0},
      .extent = presentData.extent,
    };
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    HephGui::Render(commandBuffer);
    vkCmdEndRenderPass(commandBuffer);
    vkEndCommandBuffer(commandBuffer);
  }
  VkPipelineStageFlags  waitStage[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSubmitInfo  submitInfo = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &presentData.syncObject.semaphoreAvailable,
    .pWaitDstStageMask = waitStage,
    .commandBufferCount = 1,
    .pCommandBuffers = &commandBuffer,
    .signalSemaphoreCount = 1,
    .pSignalSemaphores = &presentData.syncObject.semaphoreFinish,
  };
	VkQueue queue = m_device.queues[0].queue;
  HEPH_CHECK_RESULT(HephResult(vkQueueSubmit(queue, 1, &submitInfo, presentData.syncObject.fence)
				, "Failed to submit Post"));
  VkPresentInfoKHR  presentInfo = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &presentData.syncObject.semaphoreFinish,
    .swapchainCount = 1,
    .pSwapchains = &presentData.swapchain,
    .pImageIndices = &presentData.imageIndex,
    .pResults = nullptr,
  };
  vkQueuePresentKHR(queue, &presentInfo);
	return (HephResult());
}

void  HephGuiAppGLFW::setupCallbackForWindow(GLFWwindow *window) {
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  glfwSetWindowUserPointer(window, this);
  glfwSetWindowSizeCallback(window, s_callbackWindowSize);
  HephGLFW_init(window);
}

HephResult	HephGuiAppGLFW::hephaestusSetup(const HephGuiAppGLFWCreateInfo& createInfo) {
	uint32_t glfwExtCount = 1;
	const char **glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
  std::vector<const char*> vkExts;
  for (int i = 0; i < glfwExtCount; i++)
    vkExts.push_back(glfwExts[i]);

	std::vector<HephInstanceExtensionInterface*>	instanceExtensions;
	HephExtensionDebug                            extDebug;
	HephExtensionScreenRendering									extScreenRendering;
	instanceExtensions.push_back(&extDebug);
	instanceExtensions.push_back(&extScreenRendering);

	HephInstanceCreateInfo		instanceCreateInfo = {
		.pApplicationName = createInfo.pApplicationName,
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = createInfo.pApplicationName,
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

HephResult	HephGuiAppGLFW::createHephSwapchain() {
	VkSurfaceFormatKHR	format = (VkSurfaceFormatKHR){VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	HephSurfaceSupportDetails	surfaceSupportDetails(m_device, m_surface);
	if (surfaceSupportDetails.formats[0].format != VK_FORMAT_UNDEFINED)
    format = surfaceSupportDetails.formats[0];
  format = (VkSurfaceFormatKHR){VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  m_surfaceFormat = format;
  uint32_t minImageCount = std::min(3u, surfaceSupportDetails.capabilies.minImageCount);
  if (3 < surfaceSupportDetails.capabilies.maxImageCount && 3 > minImageCount)
    minImageCount = 3;
	HEPH_CHECK_RESULT(createRenderPass());
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

HephResult	HephGuiAppGLFW::createRenderPass() {
  VkAttachmentDescription attachmentDescription = {
    .format = m_surfaceFormat.format,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
  };
  VkAttachmentReference   colorAttachmentReference = {
    .attachment = 0,
    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };
  VkSubpassDescription    subpassDescription = {
    .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
    .colorAttachmentCount = 1,
    .pColorAttachments = &colorAttachmentReference,
  };
  VkSubpassDependency     subpassDependency = {
    .srcSubpass = VK_SUBPASS_EXTERNAL,
    .dstSubpass = 0,
    .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
      | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
  };
  VkRenderPassCreateInfo  renderPassInfo = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .attachmentCount = 1,
    .pAttachments = &attachmentDescription,
    .subpassCount = 1,
    .pSubpasses = &subpassDescription,
    .dependencyCount = 1,
    .pDependencies = &subpassDependency,
  };
	return (HephResult(vkCreateRenderPass(m_device.device, &renderPassInfo
					, m_device.pAllocationCallbacks, &m_renderPass)
					, "Failed to create RenderPass"));
}

