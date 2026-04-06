#pragma once

#include "../core/hephaestus_core.hpp"
#include "../command/hephCommandPool.hpp"
#include "../memory/hephMemoryAllocator.hpp"
#include "../plugins/font/hephFont.hpp"

namespace HephGui {

//forward declaration of HephGuiInternal.hpp type
struct  HephGuiContext;
struct  HephDrawList;
struct  HephWindow;

enum    HephGuiCol_
{
  HephGuiCol_Text,
  HephGuiCol_TextDisabled,
  HephGuiCol_WindowBg,              // Background of normal windows
  HephGuiCol_ChildBg,               // Background of child windows
  HephGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
  HephGuiCol_Border,
  HephGuiCol_BorderShadow,
  HephGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
  HephGuiCol_FrameBgHovered,
  HephGuiCol_FrameBgActive,
  HephGuiCol_TitleBg,               // Title bar
  HephGuiCol_TitleBgActive,         // Title bar when focused
  HephGuiCol_TitleBgCollapsed,      // Title bar when collapsed
  HephGuiCol_MenuBarBg,
  HephGuiCol_ScrollbarBg,
  HephGuiCol_ScrollbarGrab,
  HephGuiCol_ScrollbarGrabHovered,
  HephGuiCol_ScrollbarGrabActive,
  HephGuiCol_CheckMark,             // Checkbox tick and RadioButton circle
  HephGuiCol_SliderGrab,
  HephGuiCol_SliderGrabActive,
  HephGuiCol_Button,
  HephGuiCol_ButtonHovered,
  HephGuiCol_ButtonActive,
  HephGuiCol_COUNT,
};

class HephApp {
  public:
    virtual void  destroy() {};
    virtual void  initWindow() = 0;
    virtual void  render() = 0;
};

HephResult      init();
void            setDisplaySize(int width, int height);
HephResult      create(HephDevice& device, VkRenderPass renderPass = VK_NULL_HANDLE);
void            destroy();
HephGuiContext& getContext();

void            Render(VkCommandBuffer cmdBuffer);

HephWindow*     GetCurrentWindowPtr();
void            NewFrame();

void            SetPosCurrentWindow(glm::ivec2 pos, bool condFirstFrame = false);
void            SetSizeCurrentWindow(glm::ivec2 size, bool condFirstFrame = false);
void            SetDimensionCurrentWindow(glm::ivec2 pos, glm::ivec2 size, bool condFirstFrame = false);

void            SetWindowUserPointer(void* userPtr);
void            SetWindowKeyCallback(void (*callbackKey)(HephWindow*, int key, int scancode, int action, int mods));
void            SetWindowCharModsCallback(void (*callbackCharMods)(HephWindow*, uint32_t codepoint, int mods));
void            SetWindowCursorPosCallback(void (*callbackCursorPos)(HephWindow*, glm::vec2 pos));
void            SetWindowMouseButtonCallback(void (*callbackMouseButton)(HephWindow*, int button, int action, int mod));
void            SetWindowScrollCallback(void (*callbackScroll)(HephWindow*, double xoffset, double yoffset));
void            SetWindowDropCallback(void (*callbackDrop)(HephWindow*, int count, const char** paths));
void            SetWindowResizeCallback(void (*callbackResize)(HephWindow*, glm::ivec2 pos, glm::ivec2 size));
void            SetWindowCloseCallback(void (*callbackClose)(HephWindow*));

//bool        Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
bool            Begin(std::string name, bool* p_open = nullptr);
void            End();

}
