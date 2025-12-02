#include "hephGui.hpp"
#include "core/hephDescriptorSet.hpp"
#include "core/hephResult.hpp"
#include "memory/hephMemoryAllocator.hpp"
#include <cassert>
#include <cstdint>
#include <glm/fwd.hpp>
#include <stack>
#include <vulkan/vulkan_core.h>
#include "hephGuiInternal.hpp"

namespace HephGui {

static  HephGuiContext          guiContext;

static  std::stack<HephWindow*> winStack;

static  std::vector<HephWindow> winList;

HephResult  init() {
  return (HephPluginFont::init());
}

HephGuiContext& getContext() {
  return (guiContext);
}

HephResult  create(HephDevice& device, VkRenderPass renderPass) {
  return (guiContext.create(device, renderPass));
}

void        setDisplaySize(int width, int height) {
  guiContext.setDisplaySize(width, height);
}

void        destroy() {
  guiContext.destroy();
}

void        Render(VkCommandBuffer commandBuffer) {
  assert(winStack.empty() && "End missing for window");
  guiContext.render(commandBuffer);
}

void        NewFrame() {
  guiContext.newFrame();
}

void        SetDimensionCurrentWindow(glm::ivec2 pos, glm::ivec2 size) {
  assert(!winStack.empty() && "Cannot Set pos currentWin WinStack is empty");
  winStack.top()->pos = pos;
  winStack.top()->size = size;
}

void        SetPosCurrentWindow(glm::ivec2 pos) {
  assert(!winStack.empty() && "Cannot Set pos currentWin WinStack is empty");
  winStack.top()->pos = pos;
}

void        SetSizeCurrentWindow(glm::ivec2 size) {
  assert(!winStack.empty() && "Cannot Set size currentWin WinStack is empty");
  winStack.top()->size = size;
}

HephWindow* GetCurrentWindowPtr() {
  assert(!winStack.empty() && "No Current Window");
  return (winStack.top());
}

bool        Begin(std::string name, bool* p_open) {
  HephWindow* winPtr = nullptr;
  for (auto& win: winList) {
    if (win.name == name) {
      winPtr = &win;
    }
  }
  guiContext.drawListBuffer.push_back(HephDrawList(guiContext.getSharedData()));
  if (winPtr == nullptr) {
    winList.push_back(HephWindow(name));
    winPtr = &winList.back();
    winPtr->drawList = &guiContext.drawListBuffer.back();
    winPtr->drawList->pushClipRectFullScreen();
  }
  else {
    winPtr->firstFrame = false;
    winPtr->drawList = &guiContext.drawListBuffer.back();
    winPtr->drawList->pushClipRect(winPtr->pos, winPtr->pos + winPtr->size);
  }
  winPtr->drawList->addDrawCmd();
  winStack.push(winPtr);
  return (true);
}

void        End() {
  assert(winStack.size() > 0 && "End doesn't correspond to a Begin");
  HephWindow* winPtr = winStack.top();
  winPtr->drawList->addRect(winPtr->pos, winPtr->pos + winPtr->size, glm::vec4(0.5, 0.5, 0.5, 1.0), 2.0f);
  winPtr->drawList->addDrawCmd();
  winPtr->drawList->addRectFill(winPtr->pos, winPtr->pos + winPtr->size, glm::vec4(0.05, 0.05, 0.05, 0.7));
  winStack.pop();
}

}
