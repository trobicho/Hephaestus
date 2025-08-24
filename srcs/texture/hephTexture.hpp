#pragma once

#include "../core/hephaestus_core.hpp"
#include "../memory/hephMemoryAllocator.hpp"
#include <glm/glm.hpp>

struct  HephTextureArea {

  glm::vec2 min = glm::vec2(0.0f);
  glm::vec2 max = glm::vec2(0.0f);
};

struct  HephTextureAtlas {
  void              addArea(HephTextureArea area) {areas.push_back(area);}
  HephTextureArea   getArea(int n) const {
    if (n < areas.size())
      return (areas[n]);
    return (HephTextureArea());
  }

  HephImageWrapper              image;
  std::vector<HephTextureArea>  areas;
};
