#include "hephGuiInternal.hpp"
#include "hephGui.hpp"
#include <cstdint>

namespace HephGui {

bool  ButtonBehavior(const glm::ivec4& rect, HephGuiId id, bool* hovered, bool* held) {
  HephGuiContext& ctx = getContext();

  *hovered = (ctx.hoveredId == id);
  *held = false;

  return (id == ctx.activeId);
}

bool  Button(const std::string& text) {
  HephWindow*   window = GetCurrentWindowPtr();
  HephDrawList* drawList = window->drawList;
  glm::ivec2    textSize = drawList->getTextSize(text, -1.0f);
  HephGuiStyle& style = getContext().style;
  HephGuiId     id = HephHashStr(text.c_str());

  glm::ivec2    clipRectMin = window->frameData.cursorPos;
  glm::ivec2    clipRectMax = clipRectMin + textSize + 10;
  glm::ivec4    clipRect = glm::ivec4(clipRectMin, clipRectMax);
  glm::ivec4    clipRectText = glm::ivec4(clipRectMin + 5, clipRectMax - 5);

  window->itemAdd(clipRect, id);

  bool  hovered = false;
  bool  held = false;
  bool  pressed = ButtonBehavior(clipRect, id, &hovered, &held);

  uint32_t  col = HephGuiCol_Button;
  if (hovered)
    col = HephGuiCol_ButtonHovered;
  if (pressed)
    col = HephGuiCol_ButtonActive;

  window->drawList->addRectFill(clipRectMin, clipRectMax, style.colors[col]);
  window->drawList->addText(text, -1, clipRectText, style.colors[HephGuiCol_Text]);

  return (pressed);
}

bool  MenuItem(const std::string& text) {
  HephWindow*   window = GetCurrentWindowPtr();
  HephDrawList* drawList = window->drawList;
  glm::ivec2    textSize = drawList->getTextSize(text, -1.0f);
  HephGuiStyle& style = getContext().style;
  HephGuiId     id = HephHashStr(text.c_str());

  glm::ivec2    clipRectMin = window->frameData.cursorPos;
  glm::ivec2    clipRectMax = clipRectMin + textSize;
  glm::ivec4    clipRect = glm::ivec4(clipRectMin, clipRectMax);

  window->itemAdd(clipRect, id);

  bool  hovered = false;
  bool  held = false;
  bool  pressed = ButtonBehavior(clipRect, id, &hovered, &held);

  uint32_t  col = HephGuiCol_TextDisabled;
  if (pressed)
    col = HephGuiCol_Text;

  //window->drawList->addRectFill(clipRectMin, clipRectMax, style.colors[col]);
  window->drawList->addText(text, -1, clipRect, style.colors[col]);

  return (false);
}

}
