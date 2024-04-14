#include "hephSwapchain.hpp"
#include <vulkan/vulkan_core.h>

HephResult	HephSwapchain::create(HephDevice& device, HephSwapchainCreateInfo createInfo) {
	m_createInfo = createInfo;
	m_device =	device;
	m_extent = createInfo.swapchainCreateInfo.imageExtent;
	return (createSwapchain());
}

HephResult	HephSwapchain::destroy() {
	if (m_swapchain != VK_NULL_HANDLE) {
		for (auto& image : m_images) {
			HEPH_PRINT_RESULT(destroySwapImage(image));
		}
		vkDestroySwapchainKHR(m_device.device, m_swapchain, m_device.pAllocationCallbacks);
		m_swapchain = VK_NULL_HANDLE;
	}
	return (HephResult());
}

HephResult	HephSwapchain::acquireNextImage(HephSwapchainPresentData& presentData) {
	auto& image = m_images[m_imageCurrent];
  vkWaitForFences(m_device.device, 1, &image.fence, VK_TRUE, UINT64_MAX);
  presentData = HephSwapchainPresentData{
    .image = image,
    .extent = m_extent,
		.imageIndex = m_imageCurrent,
    .swapchain = m_swapchain,
  };
  vkResetFences(m_device.device, 1, &image.fence);
	HephResult result(vkAcquireNextImageKHR(m_device.device
      , m_swapchain, UINT64_MAX
      , image.semaphoreAvailable
      , VK_NULL_HANDLE, &presentData.imageIndex));
  m_imageCurrent = (m_imageCurrent + 1) % m_imageCount;
  return (result);
}

HephResult	HephSwapchain::createSwapchain() {
	for (auto& image : m_images) {
		HEPH_PRINT_RESULT(destroySwapImage(image));
	}
	m_createInfo.swapchainCreateInfo.oldSwapchain = m_swapchain;
	m_createInfo.swapchainCreateInfo.imageExtent = m_extent;
	HEPH_CHECK_RESULT(HephResult(vkCreateSwapchainKHR(m_device.device, &m_createInfo.swapchainCreateInfo, m_device.pAllocationCallbacks, &m_swapchain)
				, "Failed to create the Swapchain !"));
	HEPH_CHECK_RESULT(HephResult(vkGetSwapchainImagesKHR(m_device.device, m_swapchain, &m_imageCount, nullptr)
				, "Failed to retrieve Swapchain image !"));
	HEPH_CHECK_RESULT(HephResult("Empty Swapchain", (m_imageCount > 0)));
	std::vector<VkImage>	imageData;
	imageData.resize(m_imageCount);
	HEPH_CHECK_RESULT(HephResult(vkGetSwapchainImagesKHR(m_device.device, m_swapchain, &m_imageCount, imageData.data())
				, "Failed to retrieve Swapchain image !"));

	m_images.resize(m_imageCount);
	m_imageCurrent = 0;
	for (int i = 0; i < m_imageCount; i++) {
		m_images[i].image = imageData[i];
		VkImageViewCreateInfo	viewInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_images[i].image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = m_createInfo.swapchainCreateInfo.imageFormat,
			.components = m_createInfo.componentMapping,
			.subresourceRange = m_createInfo.imageSubresourceRange,
		};
		HEPH_CHECK_RESULT(HephResult(vkCreateImageView(m_device.device, &viewInfo, m_device.pAllocationCallbacks, &m_images[i].imageView)
					, "Failed to create the image views"));
		VkSemaphoreCreateInfo semaphoreInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};
    HEPH_CHECK_RESULT(HephResult(vkCreateSemaphore(m_device.device, &semaphoreInfo, m_device.pAllocationCallbacks
          , &m_images[i].semaphoreAvailable), "Failed to create Semaphores!"));
    HEPH_CHECK_RESULT(HephResult(vkCreateSemaphore(m_device.device, &semaphoreInfo, m_device.pAllocationCallbacks
          , &m_images[i].semaphoreFinish), "Failed to create Semaphores!"));
		VkFenceCreateInfo	fenceInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,
		};
    HEPH_CHECK_RESULT(HephResult(vkCreateFence(m_device.device, &fenceInfo, m_device.pAllocationCallbacks
          , &m_images[i].fence), "Failed to create Semaphores!"));
	}
	if (m_createInfo.renderPass != VK_NULL_HANDLE)
		createFramebuffers(m_createInfo.renderPass);
	return (HephResult());
}

HephResult	HephSwapchain::createFramebuffers(VkRenderPass renderPass) {
	m_createInfo.renderPass = renderPass;
	for (auto& image : m_images) {
		vkDestroyFramebuffer(m_device.device, image.framebuffer, m_device.pAllocationCallbacks);
		VkFramebufferCreateInfo	createInfo = {
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass = renderPass,
			.attachmentCount = 1,
			.pAttachments = &image.imageView,
			.width = m_extent.width,
			.height = m_extent.height,
			.layers = 1,
		};
		HEPH_CHECK_RESULT(HephResult(vkCreateFramebuffer(m_device.device, &createInfo, m_device.pAllocationCallbacks, &image.framebuffer)
					, "Failed to create Framebuffers!"));
	}
	return (HephResult());
}
