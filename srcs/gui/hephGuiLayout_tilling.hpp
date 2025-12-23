#pragma once
#include "hephGuiInternal.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace HephGui {

enum    WinTreeSplit {
  WinTreeSplit_Vertical,
  WinTreeSplit_Horizontal,
};

struct  WinTreeLayout: public HephGuiLayout {
  virtual ~WinTreeLayout() {};

  virtual bool            isLeaf() = 0;
  virtual void            swap() = 0;
  virtual WinTreeLayout*  split(HephGuiLayout* a_window, WinTreeSplit split, float ratio) = 0;
};

struct  WinTreeLeaf: public WinTreeLayout {
  WinTreeLeaf() {};
  WinTreeLeaf(HephGuiLayout* a_window): window(a_window) {window->parentLayout = this;}
  ~WinTreeLeaf() {};

  virtual bool            isLeaf() override {return (true);}
  virtual void            swap() override {};
  virtual WinTreeLayout*  split(HephGuiLayout* a_window, WinTreeSplit split, float ratio) override;

  virtual void  close(HephGuiLayout* ptr) override {
    if (parentLayout != nullptr && ptr != parentLayout) {
      parentLayout->close(this);
    }
    else if (ptr == parentLayout && window != nullptr)
      window->close(this);
  }
  virtual void  resize(HephGuiLayout* ptr, glm::ivec2 a_pos, glm::ivec2 a_size) override;
  virtual void  resizeInternal(glm::ivec2 a_pos, glm::ivec2 a_size) override {
    if (window != nullptr) {
      window->resizeInternal(a_pos, a_size);
      pos = window->pos;
      size = window->size;
    }
    else {
      pos = a_pos;
      size = a_size;
    }
  }
  virtual void    ptrChildChange(HephGuiLayout* ptr, HephGuiLayout* newPtr) override {window = newPtr;}

  HephGuiLayout*  window = nullptr;
};

struct  WinTreeBranch: public WinTreeLayout {
  WinTreeBranch() {}
  WinTreeBranch(WinTreeSplit a_splitLayout, float a_splitRatio): splitLayout(a_splitLayout), splitRatio(a_splitRatio) {
  }

  ~WinTreeBranch() {
    for (auto& child: childs)
      delete child;
  }

  virtual bool            isLeaf() override {return (false);}
  virtual void            swap() override {
    WinTreeLayout* childTmp = childs[0];
    childs[0] = childs[1];
    childs[1] = childTmp;
    splitRatio = 1.0 -  splitRatio;
    resizeInternal(pos, size);
  };
  virtual WinTreeLayout*  split(HephGuiLayout* a_window, WinTreeSplit split, float ratio) override;

  virtual void  close(HephGuiLayout* ptr) override {
    if (ptr == childs[0]) {
      childs[1]->parentLayout = parentLayout;
      childs[1]->resizeInternal(pos, size);
      childs[1] = nullptr;
      delete (this);
    }
    else if (ptr == childs[1]) {
      childs[0]->parentLayout = parentLayout;
      childs[0]->resizeInternal(pos, size);
      childs[0] = nullptr;
      delete (this);
    }
    else {
      childs[0]->close(this);
      childs[1]->close(this);
      delete (this);
    }
  }
  virtual void  resize(HephGuiLayout* ptr, glm::ivec2 a_pos, glm::ivec2 a_size) override;
  virtual void  resizeInternal(glm::ivec2 a_pos, glm::ivec2 a_size) override {
    if (splitLayout == WinTreeSplit_Horizontal) {
      childs[0]->resizeInternal(a_pos, glm::ivec2(a_size.x, a_size.y * splitRatio));
      childs[1]->resizeInternal(glm::ivec2(a_pos.x, a_pos.y + childs[0]->size.y), glm::ivec2(a_size.x, a_size.y - childs[0]->size.y));
    }
    else if (splitLayout == WinTreeSplit_Vertical) {
      childs[0]->resizeInternal(a_pos, glm::ivec2(a_size.x * splitRatio, a_size.y));
      childs[1]->resizeInternal(glm::ivec2(a_pos.x + childs[0]->size.x, a_pos.y), glm::ivec2(a_size.x - childs[0]->size.x, a_size.y));
    }
    pos = a_pos;
    size = a_size;
  }
  virtual void    ptrChildChange(HephGuiLayout* ptr, HephGuiLayout* newPtr) override {
    if (ptr == childs[0]) {
      childs[0] = static_cast<WinTreeLayout*>(newPtr);
      childs[0]->parentLayout = this;
    }
    else if (ptr == childs[1]) {
      childs[1] = static_cast<WinTreeLayout*>(newPtr);
      childs[1]->parentLayout = this;
    }
  }

  float           splitRatio = 0.5f;
  WinTreeSplit    splitLayout;
  WinTreeLayout*  childs[2];
};

}
