#pragma once

#include <cstdint>
#include "hephaestus.hpp"
#include "extensions/hephExtensionDebug.hpp"
#include "extensions/hephExtensionScreenRendering.hpp"
#include "memory/hephMemoryAllocator.hpp"
#include <memory>
#include <array>

#include "guiTest.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define	FRAMERATE	(1.0 / 24.0)

class	  HephTestApp {
	public:
		HephTestApp();

		HephResult	create();
    void  			run();
    void  			destroy();

    void  callbackWindowSize(GLFWwindow* window, int width, int height);
    void  callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void  callbackCharMods(GLFWwindow* window, uint32_t codepoint, int mods) {};
    void  callbackCursor(GLFWwindow* window, double x_pos, double y_pos) {};
    void  callbackMouseButton(GLFWwindow* window, int button, int action, int mod) {};
    void  callbackScroll(GLFWwindow* window, double xoffset, double yoffset) {};
    void  callbackPathDrop(GLFWwindow* window, int count, const char** paths) {};

	private:
    void          updateLayout();
    void          parse();
		HephResult		hephaestusSetup();
    HephResult		createHephSwapchain();
    void  				setupCallbackForWindow(GLFWwindow *window);

    HephResult		render();
    void          renderGui();
		
		GLFWwindow*		m_mainWindow;
    int						m_width, m_height;
		HephInstance	m_hephInstance;
		HephDevice		m_device;
		bool					m_quit = false;
		double				m_framerate = FRAMERATE;
		
    GuiTest       m_guiTest;


    //Vulkan Stuff
		HephSwapchain									m_swapchain;
    VkSurfaceKHR      						m_surface;
		VkSurfaceFormatKHR						m_surfaceFormat;
    HephCommandPool               m_commandPool;
		HephMemoryAllocator						m_allocator;
    uint32_t                      m_imageCount = 0;
    uint32_t                      m_imageCurrent = 0;
    std::vector<VkFence>          m_fences;
    std::vector<VkCommandBuffer>  m_commandBuffers;
};

