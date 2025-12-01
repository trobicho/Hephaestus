#include "gui/hephGuiInternal.hpp"
#include "hephGui.hpp"
#include "plugins/font/hephFont.hpp"
#include "texture/hephTexture.hpp"
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

namespace HephGui {

void  HephDrawList::addLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness) {
  pathLineTo(p1);
  pathLineTo(p2);
  pathStroke(color, thickness);
}

void  HephDrawList::addRect(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, float thickness) {
  pathLineTo(min + thickness / 2.0f);
  pathLineTo(glm::vec2(max.x - thickness / 2.0f, min.y + thickness / 2.0f));
  pathLineTo(max - thickness / 2.0f);
  pathLineTo(glm::vec2(min.x + thickness / 2.0f, max.y - thickness / 2.0f));
  pathLineTo(min + thickness / 2.0f);
  
  pathStroke(color, thickness);
}

void  HephDrawList::addRectFill(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color) {
  idxBuffer.push_back(vtxBuffer.size());
  idxBuffer.push_back(vtxBuffer.size() + 1);
  idxBuffer.push_back(vtxBuffer.size() + 3);
  idxBuffer.push_back(vtxBuffer.size() + 1);
  idxBuffer.push_back(vtxBuffer.size() + 2);
  idxBuffer.push_back(vtxBuffer.size() + 3);

  const HephTextureArea& area = _Data->whitePixelArea;

  vtxBuffer.push_back((HephVertex){.pos = min, .uv = area.min, .color = color});
  vtxBuffer.push_back((HephVertex){.pos = glm::vec2(max.x, min.y), .uv = {area.max.x, area.min.y}, .color = color});
  vtxBuffer.push_back((HephVertex){.pos = max, .uv = area.max, .color = color});
  vtxBuffer.push_back((HephVertex){.pos = glm::vec2(min.x, max.y), .uv = {area.min.x, area.max.y}, .color = color});

  drawCmdBuffer.back().elemCount += 6;
}

float HephDrawList::addGlyphRect(const HephFont* font, const glm::vec2& pos, int c, float size, const glm::vec4& color) {
  const HephFontGlyph&    glyph = font->getGlyph(c);
  const HephTextureArea&  area = font->getTextureAtlas().getArea(c);
  float                   scale = size / font->getPixelSize();

  idxBuffer.push_back(vtxBuffer.size());
  idxBuffer.push_back(vtxBuffer.size() + 1);
  idxBuffer.push_back(vtxBuffer.size() + 3);
  idxBuffer.push_back(vtxBuffer.size() + 1);
  idxBuffer.push_back(vtxBuffer.size() + 2);
  idxBuffer.push_back(vtxBuffer.size() + 3);

  glm::vec2 max(pos.x + (glyph.width + glyph.left) * scale, pos.y + (font->getPixelSize() / 2.0 + glyph.height - glyph.top) * scale);
  glm::vec2 min(pos.x + glyph.left * scale, pos.y + (font->getPixelSize() / 2.0 - glyph.top) * scale);

  vtxBuffer.push_back((HephVertex){.pos = min, .uv = area.min, .color = color});
  vtxBuffer.push_back((HephVertex){.pos = glm::vec2(max.x, min.y), .uv = {area.max.x, area.min.y}, .color = color});
  vtxBuffer.push_back((HephVertex){.pos = max, .uv = area.max, .color = color});
  vtxBuffer.push_back((HephVertex){.pos = glm::vec2(min.x, max.y), .uv = {area.min.x, area.max.y}, .color = color});
  
  drawCmdBuffer.back().elemCount += 6;

  return (glyph.advance * scale);
}

void  HephDrawList::addText(const std::string& text, float size,  const glm::vec4& clipRect, const glm::vec4& color) {
  glm::vec2       pos = glm::vec2((int)clipRect.x, (int)clipRect.y);
  const HephFont* font = _Data->font;
  float           lineSize = size;
  glm::vec2       rect = pos;

  for (int i = 0; i < text.size(); i++) {
    if (text[i] == '\n') {
      pos.y += lineSize;
      rect.y = pos.y;
      pos.x = clipRect.x;
    }
    if (pos.x >= clipRect.x + clipRect.z)
      continue;
    if (pos.y >= clipRect.y + clipRect.w)
      break;

    pos.x += addGlyphRect(font, pos, text[i], size, color);
    if (rect.x < pos.x)
      rect.x = pos.x;
  }
}

void  HephDrawList::addPolyline(const glm::vec2* points, uint32_t size, const glm::vec4& color, float thickness) {
  for (int i = 0; i < size - 1; i++) {
    glm::vec2 nTmp = points[i] - points[i + 1];
    glm::vec2 n1 = glm::normalize(glm::vec2(-nTmp.y, nTmp.x)) * thickness / 2.0f;
    glm::vec2 n2 = glm::normalize(glm::vec2(nTmp.y, -nTmp.x)) * thickness / 2.0f;

    glm::vec2 p1 = points[i] + n1;
    glm::vec2 p2 = points[i + 1] + n1;
    glm::vec2 p3 = points[i + 1] + n2;
    glm::vec2 p4 = points[i] + n2;

    idxBuffer.push_back(vtxBuffer.size());
    idxBuffer.push_back(vtxBuffer.size() + 1);
    idxBuffer.push_back(vtxBuffer.size() + 3);
    idxBuffer.push_back(vtxBuffer.size() + 1);
    idxBuffer.push_back(vtxBuffer.size() + 2);
    idxBuffer.push_back(vtxBuffer.size() + 3);

    vtxBuffer.push_back((HephVertex){.pos = p1, .uv = _Data->whitePixelArea.min, .color = color});
    vtxBuffer.push_back((HephVertex){.pos = p2, .uv = glm::vec2(_Data->whitePixelArea.max.x, _Data->whitePixelArea.min.y), .color = color});
    vtxBuffer.push_back((HephVertex){.pos = p3, .uv = _Data->whitePixelArea.max, .color = color});
    vtxBuffer.push_back((HephVertex){.pos = p4, .uv = glm::vec2(_Data->whitePixelArea.min.x, _Data->whitePixelArea.max.y), .color = color});

    drawCmdBuffer.back().elemCount += 6;
  }
}

void  HephDrawList::pushClipRect(const glm::ivec2& clipRect_min, const glm::ivec2& clipRect_max, bool intersect_with_current_clipRect) {
  glm::ivec4 cr = glm::ivec4(clipRect_min.x, clipRect_min.y, clipRect_max.x, clipRect_max.y);

  if (intersect_with_current_clipRect) {
    glm::ivec4 current = _ClipRectStack.top();
    if (cr.x < current.x)
      cr.x = current.x;
    if (cr.y < current.y)
      cr.y = current.y;
    if (cr.x < current.z)
      cr.z = current.z;
    if (cr.w > current.w)
      cr.w = current.w;
  }

  cr.z = std::max(cr.x, cr.z);
  cr.w = std::max(cr.y, cr.w);

  _ClipRectStack.push(cr);
}

void  HephDrawList::pushClipRectFullScreen() {
  _ClipRectStack.push(_Data->clipRectFullScreen);
}

void  HephDrawList::popClipRect() {
  _ClipRectStack.pop();
  if (_ClipRectStack.empty())
    pushClipRectFullScreen();
}

}
