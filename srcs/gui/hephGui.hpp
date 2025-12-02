#pragma once

#include "../core/hephaestus_core.hpp"
#include "../command/hephCommandPool.hpp"
#include "../memory/hephMemoryAllocator.hpp"
#include "../plugins/font/hephFont.hpp"
#include "hephGuiInternal.hpp"

namespace HephGui {

struct  HephWindow {
  HephWindow(std::string name): name(name) {};

  HephDrawList* drawList = nullptr;

  std::string   name;
  glm::ivec2    pos = {0, 0};
  glm::ivec2    size = {-1, -1};
  bool          firstFrame = true;
};

HephResult      init();
void            setDisplaySize(int width, int height);
HephResult      create(HephDevice& device, VkRenderPass renderPass = VK_NULL_HANDLE);
void            destroy();
HephGuiContext& getContext();

void        Render(VkCommandBuffer cmdBuffer);

HephWindow* GetCurrentWindowPtr();
void        NewFrame();
void        SetPosCurrentWindow(glm::ivec2 pos);
void        SetSizeCurrentWindow(glm::ivec2 size);
void        SetDimensionCurrentWindow(glm::ivec2 pos, glm::ivec2 size);

//bool        Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
bool        Begin(std::string name, bool* p_open = nullptr);
void        End();



}
