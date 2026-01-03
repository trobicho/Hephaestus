#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "hephGui.hpp"
#include "../hephaestus.hpp"
#include "../extensions/hephExtensionDebug.hpp"
#include "../extensions/hephExtensionScreenRendering.hpp"

#define	FRAMERATE_DEFAULT	(1.0 / 60.0)

struct HephGuiAppGLFWCreateInfo {
	const char*		  pApplicationName;
  int             windowWidth;
  int             windowHeight;
  double          framerate = FRAMERATE_DEFAULT;
};

class HephGuiAppGLFW {
  public:
    HephResult	  create(const HephGuiAppGLFWCreateInfo& createInfo);
    void  			  run();
    void  			  destroy();
    void          quit() {m_quit = true;}
    HephResult		renderGui();

    void          callbackWindowSize(GLFWwindow* window, int width, int height);

  private:
    void          updateLayout();
    HephResult	  hephaestusSetup(const HephGuiAppGLFWCreateInfo& createInfo);
    HephResult		createHephSwapchain();
    HephResult		createRenderPass();
    void  				setupCallbackForWindow(GLFWwindow *window);

    HephResult		render();
		
		GLFWwindow*		                m_mainWindow = nullptr;
    int                           m_width = 0;
    int                           m_height = 0;
		bool					                m_quit = false;
		double				                m_framerate = FRAMERATE_DEFAULT;
		
    //Vulkan Stuff
		HephInstance	                m_hephInstance;
		HephDevice		                m_device;
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
