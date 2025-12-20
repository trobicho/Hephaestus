#include "hephGuiIoGLFW.hpp"
#include "hephGui.hpp"
#include "hephGuiInternal.hpp"
#include <GLFW/glfw3.h>

static  HephGLFW_userCallbacks  userCallbacks;

static void s_callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (userCallbacks.callbackKey != nullptr)
    userCallbacks.callbackKey(window, key, scancode, action, mods);
  HephGui::getContext().callbackKey(key, scancode, action, mods);
}

static void s_callbackCharMods(GLFWwindow* window, uint32_t codepoint, int mods) {
  if (userCallbacks.callbackCharMods != nullptr)
    userCallbacks.callbackCharMods(window, codepoint, mods);
  HephGui::getContext().callbackCharMods(codepoint, mods);
}

static void s_callbackCursor(GLFWwindow* window, double x_pos, double y_pos) {
  if (userCallbacks.callbackCursor != nullptr)
    userCallbacks.callbackCursor(window, x_pos, y_pos);
  HephGui::getContext().callbackCursor(x_pos, y_pos);
}

static void s_callbackMouseButton(GLFWwindow* window, int button, int action, int mod) {
  if (userCallbacks.callbackMouseButton != nullptr)
    userCallbacks.callbackMouseButton(window, button, action, mod);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  HephGui::getContext().callbackMouseButton(xpos, ypos, button, action, mod);
}

static void s_callbackScroll(GLFWwindow* window, double xoffset, double yoffset) {
  if (userCallbacks.callbackScroll != nullptr)
    userCallbacks.callbackScroll(window, xoffset, yoffset);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  HephGui::getContext().callbackScroll(xpos, ypos, xoffset, yoffset);
}

using namespace HephGui;

void  HephGLFW_init(GLFWwindow *window) {
  userCallbacks.callbackKey = glfwSetKeyCallback(window, s_callbackKey);
  userCallbacks.callbackCharMods = glfwSetCharModsCallback(window, s_callbackCharMods);
  userCallbacks.callbackCursor = glfwSetCursorPosCallback(window, s_callbackCursor);
  userCallbacks.callbackMouseButton = glfwSetMouseButtonCallback(window, s_callbackMouseButton);
  userCallbacks.callbackScroll = glfwSetScrollCallback(window, s_callbackScroll);

  userCallbacks.MouseCursors[HephGuiCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
  userCallbacks.MouseCursors[HephGuiCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
}

void  HephGLFW_render(GLFWwindow *window) {
  HephGuiCursorType cursor = HephGui::getContext().cursor.type;
  glfwSetCursor(window, userCallbacks.MouseCursors[cursor] ? userCallbacks.MouseCursors[cursor] : userCallbacks.MouseCursors[HephGuiCursor_Arrow]); 
}

void  HephGLFW_destroy() {
  for (int i = 0; i < HephGuiCursor_COUNT; i++) {
    glfwDestroyCursor(userCallbacks.MouseCursors[i]);
  }
}
