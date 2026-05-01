#include "hephTestApp.hpp"
#include "thread"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <filesystem>
#include "core/hephResult.hpp"
#include "gui/hephGuiIoGLFW.hpp"
#include <iostream>
#include <iterator>
#include <memory>
#include <vulkan/vulkan_core.h>

void  HephTestApp::callbackWindowSize(GLFWwindow* window, int width, int height) {
	vkDeviceWaitIdle(m_device.device);
	HEPH_PRINT_RESULT(HephResult(vkQueueWaitIdle(m_device.queues[0].queue), "Error waiting for queue {{}}!"));
  m_width = width;
  m_height = height;

	m_swapchain.destroy();
  vkDestroySurfaceKHR(m_hephInstance.vulkanInstance, m_surface, m_device.pAllocationCallbacks);
  HEPH_PRINT_RESULT(HephResult(glfwCreateWindowSurface(m_hephInstance.vulkanInstance, m_mainWindow
        , m_device.pAllocationCallbacks, &m_surface), "Failed to create Surface {{}} !"));
  m_swapchain.recreate(VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)}, m_surface);

  HephGui::setDisplaySize(m_width, m_height);
}

void  HephTestApp::callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action != GLFW_PRESS && action != GLFW_REPEAT) {
    return ;
  }

	if (mods & GLFW_MOD_CONTROL && key == GLFW_KEY_Q) {
		m_quit = true;
	}
}

HephTestApp::HephTestApp() {
  glfwInit();
}

HephResult	HephTestApp::create() {
	HEPH_CHECK_RESULT(hephaestusSetup());

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  m_mainWindow = glfwCreateWindow(1920, 1080, "HephTest", NULL, NULL);
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

  HEPH_CHECK_RESULT(HephGui::init());
  HephGui::setDisplaySize(m_width, m_height);
  HEPH_CHECK_RESULT(HephGui::create(m_device, m_renderPass));

	return (HephResult());
}

void  HephTestApp::destroy() {
  std::cout << "DESTROY" << std::endl;

  HephGLFW_destroy();
	vkDeviceWaitIdle(m_device.device);
  m_guiTest.destroy();
  HephGui::destroy();
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

void  HephTestApp::run() {
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

void        HephTestApp::renderGui() {
  HephGui::NewFrame();
  HephGui::Begin("test");
  m_guiTest.initWindow();
  HephGui::SetDimensionCurrentWindow(glm::ivec2(0.0, 0.0), glm::ivec2(m_width, m_height));
  m_guiTest.render();
  HephGui::End();
}

HephResult	HephTestApp::render() {
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

  renderGui();
  HephGLFW_render(m_mainWindow);

  vkBeginCommandBuffer(commandBuffer, &beginInfo);
  {
    VkClearValue clearValue = (VkClearValue){0.0f, 0.0f, 0.0f, 1.0f};
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

static void s_callbackWindowSize(GLFWwindow* window, int width, int height) {
  HephTestApp *hephTestPtr = static_cast<HephTestApp*>(glfwGetWindowUserPointer(window));
  hephTestPtr->callbackWindowSize(window, width, height);
}

static void s_callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  HephTestApp *hephTestPtr = static_cast<HephTestApp*>(glfwGetWindowUserPointer(window));
  hephTestPtr->callbackKey(window, key, scancode, action, mods);
}

static void s_callbackCharMods(GLFWwindow* window, uint32_t codepoint, int mods) {
  HephTestApp *hephTestPtr = static_cast<HephTestApp*>(glfwGetWindowUserPointer(window));
  hephTestPtr->callbackCharMods(window, codepoint, mods);
}

static void s_callbackCursor(GLFWwindow* window, double x_pos, double y_pos) {
  HephTestApp *hephTestPtr = static_cast<HephTestApp*>(glfwGetWindowUserPointer(window));
  hephTestPtr->callbackCursor(window, x_pos, y_pos);
}

static void s_callbackMouseButton(GLFWwindow* window, int button, int action, int mod) {
  HephTestApp *hephTestPtr = static_cast<HephTestApp*>(glfwGetWindowUserPointer(window));
  hephTestPtr->callbackMouseButton(window, button, action, mod);
}

static void s_callbackScroll(GLFWwindow* window, double xoffset, double yoffset) {
  HephTestApp *hephTestPtr = static_cast<HephTestApp*>(glfwGetWindowUserPointer(window));
  hephTestPtr->callbackScroll(window, xoffset, yoffset);
}

static void s_callbackPathDrop(GLFWwindow* window, int count, const char** paths) {
  HephTestApp *hephTestPtr = static_cast<HephTestApp*>(glfwGetWindowUserPointer(window));
  hephTestPtr->callbackPathDrop(window, count, paths);
}

void  HephTestApp::setupCallbackForWindow(GLFWwindow *window) {
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  glfwSetWindowUserPointer(window, this);
  glfwSetWindowSizeCallback(window, s_callbackWindowSize);
  glfwSetKeyCallback(window, s_callbackKey);
  glfwSetCharModsCallback(window, s_callbackCharMods);
  glfwSetCursorPosCallback(window, s_callbackCursor);
  glfwSetMouseButtonCallback(window, s_callbackMouseButton);
  glfwSetScrollCallback(window, s_callbackScroll);
  glfwSetDropCallback(window, s_callbackPathDrop);
  HephGLFW_init(window);
}

HephResult	HephTestApp::hephaestusSetup() {
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
		.pApplicationName = "HephTest",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = "HephTest",
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

HephResult	HephTestApp::createHephSwapchain() {
	VkSurfaceFormatKHR	format = (VkSurfaceFormatKHR){VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	HephSurfaceSupportDetails	surfaceSupportDetails(m_device, m_surface);
	if (surfaceSupportDetails.formats[0].format != VK_FORMAT_UNDEFINED)
		format = surfaceSupportDetails.formats[0];
	m_surfaceFormat = format;
	HEPH_CHECK_RESULT(createRenderPass());
	HephSwapchainCreateInfo		swapchainCreateInfo;
	VkSwapchainCreateInfoKHR	swapInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = m_surface,
		.minImageCount = std::min(3u, surfaceSupportDetails.capabilies.minImageCount),
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

HephResult	HephTestApp::createRenderPass() {
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

int	main(int ac, char** av) {
	HephTestApp   hephTest;
	HephResult	  result = hephTest.create();

	HEPH_PRINT_RESULT(result);

	if (!result.valid()) {
		return (1);
	}

	hephTest.run();
	hephTest.destroy();
	return (0);
}
