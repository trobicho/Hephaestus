#pragma once

#include <GLFW/glfw3.h>
#include "hephGuiInternal.hpp"

struct  HephGLFW_userCallbacks {
  void  (*callbackKey)(GLFWwindow*, int key, int scancode, int action, int mods)  = nullptr;
  void  (*callbackCharMods)(GLFWwindow*, uint32_t codepoint, int mods)            = nullptr;
  void  (*callbackCursor)(GLFWwindow*, double x_pos, double y_pos)                = nullptr;
  void  (*callbackMouseButton)(GLFWwindow*, int button, int action, int mod)      = nullptr;
  void  (*callbackScroll)(GLFWwindow*, double xoffset, double yoffset)            = nullptr;
  void  (*callbackDrop)(GLFWwindow*, int count, const char** paths)               = nullptr;

  GLFWcursor*   MouseCursors[HephGui::HephGuiCursor_COUNT];
};

void  HephGLFW_init(GLFWwindow *window);
void  HephGLFW_render(GLFWwindow *window);
void  HephGLFW_destroy();
