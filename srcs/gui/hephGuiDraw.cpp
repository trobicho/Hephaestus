#include "hephGui.hpp"
#include <cstdint>

namespace HephGui {

void  HephDrawList::addLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness) {
  pathLineTo(p1);
  pathLineTo(p2);
  pathStroke(color, thickness);
}

void  HephDrawList::addRect(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, float thickness) {
  pathLineTo(min);
  pathLineTo(glm::vec2(max.x, min.y));
  pathLineTo(max);
  pathLineTo(glm::vec2(min.x, max.y));
  pathLineTo(min);
  pathStroke(color, thickness);
}

void  HephDrawList::addRectFill(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color) {
  idxBuffer.push_back(vtxBuffer.size());
  idxBuffer.push_back(vtxBuffer.size() + 1);
  idxBuffer.push_back(vtxBuffer.size() + 3);
  idxBuffer.push_back(vtxBuffer.size() + 1);
  idxBuffer.push_back(vtxBuffer.size() + 2);
  idxBuffer.push_back(vtxBuffer.size() + 3);

  vtxBuffer.push_back((HephGuiVertex){.pos = min, .color = color});
  vtxBuffer.push_back((HephGuiVertex){.pos = glm::vec2(max.x, min.y), .color = color});
  vtxBuffer.push_back((HephGuiVertex){.pos = glm::vec2(max.x, max.y), .color = color});
  vtxBuffer.push_back((HephGuiVertex){.pos = glm::vec2(min.x, max.y), .color = color});
}

void  HephDrawList::addPolyLine(const glm::vec2* points, uint32_t size, const glm::vec4& color, float thickness) {
  for (int i = 0; i < size - 1; i++) {
    glm::vec2 min = points[i] - (float)(thickness / 2.0);
    glm::vec2 max = points[i + 1] + (float)(thickness / 2.0);

    idxBuffer.push_back(vtxBuffer.size());
    idxBuffer.push_back(vtxBuffer.size() + 1);
    idxBuffer.push_back(vtxBuffer.size() + 3);
    idxBuffer.push_back(vtxBuffer.size() + 1);
    idxBuffer.push_back(vtxBuffer.size() + 2);
    idxBuffer.push_back(vtxBuffer.size() + 3);

    vtxBuffer.push_back((HephGuiVertex){.pos = min, .color = color});
    vtxBuffer.push_back((HephGuiVertex){.pos = glm::vec2(max.x, min.y), .color = color});
    vtxBuffer.push_back((HephGuiVertex){.pos = glm::vec2(max.x, max.y), .color = color});
    vtxBuffer.push_back((HephGuiVertex){.pos = glm::vec2(min.x, max.y), .color = color});
  }
}

}
