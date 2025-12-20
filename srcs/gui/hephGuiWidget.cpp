#include "hephGuiInternal.hpp"
#include "hephGui.hpp"

namespace HephGui {

bool  Button(const std::string& text) {
  HephWindow*   window = GetCurrentWindowPtr();
  HephDrawList* drawList = window->drawList;
  glm::vec2     textSize = drawList->getTextSize(text, -1.0f);

  glm::vec2     clipRectMin = drawList->getClipRectMin();
  glm::vec2     clipRectMax = clipRectMin + textSize + 10.f;
  window->drawList->addRectFill(clipRectMin, clipRectMax, const glm::vec4 &color);
}

}
