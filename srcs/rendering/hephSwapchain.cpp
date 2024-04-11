#include "hephSwapchain.hpp"
#include <vulkan/vulkan_core.h>

HephResult	HephSwapchain::create(HephDevice& device, HephSwapchainCreateInfo createInfo) {
	m_createInfo = createInfo;
	m_device =	device;
	m_extent = createInfo.swapchainCreateInfo.imageExtent;
	return (createSwapchain());
}

HephResult	HephSwapchain::destroy() {
	return (HephResult());
}

HephResult	HephSwapchain::createSwapchain() {
	m_createInfo.swapchainCreateInfo.oldSwapchain = m_swapchain;
	HEPH_CHECK_RESULT(HephResult(vkCreateSwapchainKHR(m_device.device, &m_createInfo.swapchainCreateInfo, m_device.pAllocationCallbacks, &m_swapchain)
				, "Failed to create the Swapchain !"));
	HEPH_CHECK_RESULT(HephResult(vkGetSwapchainImagesKHR(m_device.device, m_swapchain, &m_imageCount, nullptr)
				, "Failed to retrieve Swapchain image !"));
	HEPH_CHECK_RESULT(HephResult("Empty Swapchain", (m_imageCount > 0)));
	std::vector<VkImage>	imageData(m_imageCount);
	HEPH_CHECK_RESULT(HephResult(vkGetSwapchainImagesKHR(m_device.device, m_swapchain, &m_imageCount, imageData.data())
				, "Failed to retrieve Swapchain image !"));

	m_images.resize(m_imageCount);
	for (int i = 0; i < m_imageCount; i++) {
		VkImageViewCreateInfo	viewInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_images[i].image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = m_createInfo.swapchainCreateInfo.imageFormat,
			.components = m_createInfo.componenMapping,
			.subresourceRange = m_createInfo.imageSubresourceRange,
		};
		HEPH_CHECK_RESULT(HephResult(vkCreateImageView(m_device.device, &viewInfo, m_device.pAllocationCallbacks, &m_images[i].imageView)
					, "Failed to create the image views"));
		m_images[i].image = imageData[i];
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
