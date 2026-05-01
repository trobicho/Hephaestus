#include "guiTest.hpp"
#include "gui/hephGuiInternal.hpp"
#include <cstdint>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include "gui/hephGui.hpp"

using namespace HephGui;

/*
void      Graph::generateLayoutVertical(glm::vec2 spaceBetweenNode) {
}
*/

static void  s_callbackKey(HephWindow* window, int key, int scancode, int action, int mods) {
  GuiTest *guiTestPtr = static_cast<GuiTest*>(window->userPtr);
  guiTestPtr->callbackKey(window, key, scancode, action, mods);
}
static void  s_callbackCursor(HephWindow* window, glm::vec2 pos) {
  GuiTest *guiTestPtr = static_cast<GuiTest*>(window->userPtr);
  guiTestPtr->callbackCursor(window, pos);
}
static void  s_callbackMouseButton(HephWindow* window, int button, int action, int mod) {
  GuiTest *guiTestPtr = static_cast<GuiTest*>(window->userPtr);
  guiTestPtr->callbackMouseButton(window, button, action, mod);
}
static void  s_callbackScroll(HephWindow* window, double xoffset, double yoffset) {
  GuiTest *guiTestPtr = static_cast<GuiTest*>(window->userPtr);
  guiTestPtr->callbackScroll(window, xoffset, yoffset);
}
static void  s_callbackResize(HephGui::HephWindow* window, glm::ivec2 pos, glm::ivec2 size) {
  GuiTest *guiTestPtr = static_cast<GuiTest*>(window->userPtr);
  guiTestPtr->callbackResize(window, pos, size);
}
static void  s_callbackDrawListRender(HephDrawList* drawList, VkCommandBuffer commandBuffer) {
  GuiTest *guiTestPtr = static_cast<GuiTest*>(drawList->userPtr);
  guiTestPtr->callbackDrawListRender(drawList, commandBuffer);
}

void  GuiTest::callbackKey(HephGui::HephWindow* window, int a_key, int scancode, int action, int mods) {
}

void  GuiTest::callbackCursor(HephGui::HephWindow* window, glm::vec2 pos) {
}

void  GuiTest::callbackMouseButton(HephGui::HephWindow* window, int button, int action, int mod) {
}

void  GuiTest::callbackScroll(HephGui::HephWindow* window, double xoffset, double yoffset) {
}

void  GuiTest::callbackResize(HephGui::HephWindow* window, glm::ivec2 pos, glm::ivec2 size) {
}

void  GuiTest::callbackDrawListRender(HephDrawList* drawList, VkCommandBuffer commandBuffer) {
}

void      GuiTest::initWindow() {
  HephGui::SetWindowUserPointer(this);
  HephGui::SetWindowKeyCallback(s_callbackKey);
  HephGui::SetWindowMouseButtonCallback(s_callbackMouseButton);
  HephGui::SetWindowScrollCallback(s_callbackScroll);
  HephGui::SetWindowCursorPosCallback(s_callbackCursor);
  HephGui::SetWindowResizeCallback(s_callbackResize);
}

void      GuiTest::render() {
  const HephWindow*   winPtr = HephGui::GetCurrentWindowPtr();

  //if (HephGui::getContext().getFocusedWindowPtr() != winPtr)
    //m_drag = false;

  HephDrawList&       drawList = *winPtr->drawList;

  glm::vec2 clipRectMin = drawList.getClipRectMin();
  glm::vec2 clipRectMax = drawList.getClipRectMax();

  m_drawList.newFrame();
  //m_drawList.userDrawListRender = s_callbackDrawListRender;
  //m_drawList.userPtr = this;

  m_drawList.pushClipRect(drawList.getClipRectMin(), drawList.getClipRectMax());
  //HephGui::getContext().userDrawListBuffer.push_back(&m_drawList);

  //Button("testjkfdjkf");
  Button("salut 1");
  MenuItem("salut 2");
  MenuItem("salut 3");
  MenuItem("salut 4");
  Button("salut 5");
  /*
  if (m_showInfo) {
    uint32_t  totalNodes = 0;
    uint32_t  totalEdges = 0;

    std::string strInfo =
      "totalNodes: " + std::to_string(totalNodes)
      + "\ntotalEdges: " + std::to_string(totalEdges)
      + "\ntranslate: [" + std::to_string(m_translate.x) + ", " + std::to_string(m_translate.y) + "]"
      + "\nscale: " + std::to_string(m_scale.y);
    glm::vec2 infoSize = drawList.getTextSize(strInfo, -1.0);

    drawList.addRectFill(clipRectMin + glm::vec2(3.0, 3.0), clipRectMin + glm::vec2(9.0, 6.0) + infoSize, glm::vec4(0.2, 0.2, 0.2, 1.0));
    drawList.addRect(clipRectMin + glm::vec2(3.0, 3.0), clipRectMin + glm::vec2(9.0, 6.0) + infoSize, glm::vec4(0.7, 0.7, 0.7, 1.0), 3.0f);
    drawList.addText(strInfo, -1.0, glm::vec4(clipRectMin.x + 5.0, clipRectMin.y + 5.0, clipRectMax.x, clipRectMax.y)
        , glm::vec4(0.8, 0.8, 0.8, 1.0));
  }
  */
}
