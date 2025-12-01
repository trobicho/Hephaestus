#pragma once

#include "../core/hephaestus_core.hpp"
#include "../memory/hephMemoryAllocator.hpp"
#include <glm/glm.hpp>

struct  HephTextureArea {
  glm::vec2 min = glm::vec2(0.0f);
  glm::vec2 max = glm::vec2(0.0f);
};

struct  HephTextureAtlas {
  void                      addArea(HephTextureArea area) {areas.push_back(area);}
  const HephTextureArea&    getArea(int n) const {
    assert(n < areas.size() && "character is not in fontAtlas");
    return (areas[n]);
  }

  HephImageWrapper              image;
  std::vector<HephTextureArea>  areas;
};
