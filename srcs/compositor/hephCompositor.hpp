#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../gui/hephGui.hpp"
#include "../hephaestus.hpp"
#include "../extensions/hephExtensionDebug.hpp"
#include "../extensions/hephExtensionScreenRendering.hpp"
#include "../extensions/hephExtensionDynamicRendering.hpp"

struct  HephCompositorOutputCreateInfo {
  HephCompositorOutputCreateInfo(HephInstance& hephInstance, HephDevice& device): hephInstance(hephInstance), device(device) {}
  const char*   name;
  int           width;
  int           height;
  float         fps = 60.0;
  HephInstance& hephInstance;
  HephDevice&		device;
};

class HephCompositorOutput {
  public:
    HephCompositorOutput(const HephCompositorOutputCreateInfo& createInfo);
    void  callbackWindowSize(GLFWwindow* window, int width, int height);

  private:
    HephResult  create();
    HephResult  createSwapchain();

    std::string                   m_outputName = "";
    float                         m_fps = 60.0;
		GLFWwindow*		                m_glfwWindow = nullptr;
    int                           m_width = 0;
    int                           m_height = 0;

    HephInstance&                 m_hephInstance;
    HephDevice&		                m_device;
		HephSwapchain									m_swapchain;
    VkSurfaceKHR      						m_surface;
		VkSurfaceFormatKHR						m_surfaceFormat;
    VkRenderPass                  m_renderPass = VK_NULL_HANDLE;
    HephCommandPool               m_commandPool;
		HephMemoryAllocator						m_allocator;
    uint32_t                      m_imageCount = 0;
    uint32_t                      m_imageCurrent = 0;
    std::vector<VkFence>          m_fences;
    std::vector<VkCommandBuffer>  m_commandBuffers;
};

class HephCompositor {
  public:
    HephCompositor() {glfwInit(); HephGui::init();}

    HephResult    create();
    HephResult    destroy();

  private:
    HephResult	  instanceSetup();

		HephInstance	                      m_hephInstance;
		HephDevice		                      m_device;
    std::vector<HephCompositorOutput>   m_outputs;
};
