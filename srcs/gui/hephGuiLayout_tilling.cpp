#include "hephGuiLayout_tilling.hpp"
#include <glm/fwd.hpp>

namespace HephGui {

WinTreeLayout*  WinTreeLeaf::split(HephGuiLayout* a_window, WinTreeSplit split, float ratio) {
  WinTreeLeaf*    newLeaf = new WinTreeLeaf(a_window);
  WinTreeBranch*  branch = new WinTreeBranch(split, ratio);
  if (parentLayout != nullptr) {
    parentLayout->ptrChildChange(this, branch);
  }
  branch->childs[0] = this;
  branch->childs[1] = newLeaf;
  branch->childs[0]->parentLayout = branch;
  branch->childs[1]->parentLayout = branch;
  branch->parentLayout = parentLayout;
  branch->resizeInternal(pos, size);
  return (branch);
}

WinTreeLayout*  WinTreeBranch::split(HephGuiLayout* a_window, WinTreeSplit split, float ratio) {
  WinTreeLeaf*    newLeaf = new WinTreeLeaf(a_window);
  WinTreeBranch*  branch = new WinTreeBranch(split, ratio);
  if (parentLayout != nullptr) {
    parentLayout->ptrChildChange(this, branch);
  }
  branch->childs[0] = this;
  branch->childs[1] = newLeaf;
  branch->childs[0]->parentLayout = branch;
  branch->childs[1]->parentLayout = branch;
  branch->parentLayout = parentLayout;
  branch->resizeInternal(pos, size);
  return (branch);
}

void  WinTreeLeaf::resize(HephGuiLayout* ptr, glm::ivec2 a_pos, glm::ivec2 a_size) {
  if (parentLayout != nullptr && ptr != parentLayout) {
    parentLayout->resize(this, a_pos, a_size);
  }
  else {
    resizeInternal(a_pos, a_size);
  }
}

void  WinTreeBranch::resize(HephGuiLayout* ptr, glm::ivec2 a_pos, glm::ivec2 a_size) {
  if (parentLayout == nullptr || ptr == parentLayout || ptr == nullptr || ptr == this) {
    resizeInternal(a_pos, a_size);
    return ;
  }

  int nodeId = (ptr == childs[0]) ? 0 : 1;
  glm::ivec2  newSize = size;
  glm::ivec2  newPos = pos;

  if (splitLayout == WinTreeSplit_Horizontal) {
    newPos.x = a_pos.x;
    newSize.x = a_size.x;
    if (nodeId == 0) {
      newPos.y = a_pos.y;
      if (a_size.y > size.y)
        newSize.y = a_size.y;
      splitRatio = (float)a_size.y / (float)newSize.y;
    }
    else if (nodeId == 1) {
      if (newPos.y < pos.y)
        newPos.y = a_pos.y;
      newSize.y = (a_pos.y - newPos.y) + a_size.y;
      splitRatio = 1.0 - ((float)a_size.y / (float)newSize.y);
    }
  }
  else if (splitLayout == WinTreeSplit_Vertical) {
    newPos.y = a_pos.y;
    newSize.y = a_size.y;
    if (nodeId == 0) {
      newPos.x = a_pos.x;
      if (a_size.x > size.x)
        newSize.x = a_size.x;
      splitRatio = (float)a_size.x / (float)newSize.x;
    }
    else if (nodeId == 1) {
      if (newPos.y < size.x)
        newPos.y = a_pos.x;
      newSize.y = (a_pos.x - newPos.x) + a_size.x;
      splitRatio = 1.0 - ((float)a_size.x / (float)newSize.x);
    }
  }

  if (parentLayout == nullptr || (pos == newPos && size == newSize))
    resizeInternal(newPos, newSize);
  else
    parentLayout->resize(this, pos, size);
}

}
