#include "hephGuiInternal.hpp"
#include "gui/hephGui.hpp"
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

namespace HephGui {

void        HephGuiContext::updateCursor() {
  float minDist = 100.f;
  int side = 0;

  cursor.type = HephGuiCursor_Arrow;
  cursor.resizeWin = nullptr;
  for (auto& win: winList) {
    if (cursor.pos.x < win.pos.x - 3.0f || cursor.pos.y < win.pos.y - 3.0f
        || cursor.pos.x > win.pos.x + win.size.x + 3.0f || cursor.pos.y > win.pos.y + win.size.y + 3.0f) {
      continue ;
    }

    float distN = cursor.pos.y - win.pos.y;
    if (distN < 0.0)
      distN = -distN;
    else
      distN -= 1.5;
    float distS = cursor.pos.y - (win.pos.y + win.size.y);
    if (distS < 0.0)
      distS = (-distS) - 1.5;
    float distW = cursor.pos.x - win.pos.x;
    if (distW < 0.0)
      distW = -distW;
    else
      distW -= 1.5;
    float distE = cursor.pos.x - (win.pos.x + win.size.x);
    if (distE < 0.0)
      distE = (-distE) - 1.5;

    if (distN <= 3.0f && distN < minDist) {
      minDist = distN;
      side = WINDOW_BORDER_NORTH;
      cursor.resizeWin = &win;
      cursor.type = HephGuiCursor_ResizeNS;
      if (distW <= 3.0f) {
        cursor.type = HephGuiCursor_ResizeNWSE;
        side |= WINDOW_BORDER_WEST;
      }
      if (distE <= 3.0f) {
        cursor.type = HephGuiCursor_ResizeNESW;
        side |= WINDOW_BORDER_EAST;
      }
    }
    else if (distS <= 3.0f && distS < minDist) {
      minDist = distS;
      side = WINDOW_BORDER_SOUTH;
      cursor.resizeWin = &win;
      cursor.type = HephGuiCursor_ResizeNS;
      if (distW <= 3.0f) {
        cursor.type = HephGuiCursor_ResizeNESW;
        side |= WINDOW_BORDER_WEST;
      }
      if (distE <= 3.0f) {
        cursor.type = HephGuiCursor_ResizeNWSE;
        side |= WINDOW_BORDER_EAST;
      }
    }
    else if (distW <= 3.0f && distW < minDist) {
      minDist = distW;
      side = WINDOW_BORDER_WEST;
      cursor.resizeWin = &win;
      cursor.type = HephGuiCursor_ResizeEW;
    }
    else if (distE <= 3.0f && distE < minDist) {
      minDist = distE;
      side = WINDOW_BORDER_EAST;
      cursor.resizeWin = &win;
      cursor.type = HephGuiCursor_ResizeEW;
    }
  }
  if (cursor.resizeWin)
    cursor.side = side;
  else
    cursor.drag = false;
}

void        HephWindow::resizeFromCursor(int side, glm::ivec2 vec) {
  glm::vec2 newPos = pos;
  glm::vec2 displaySize = getContext().displaySize;

  if (vec.x < 0.0)
    vec.x = 0.0;
  else if (vec.x > displaySize.x)
    vec.x = displaySize.x;
  if (vec.y < 0.0)
    vec.y = 0.0;
  else if (vec.y > displaySize.y)
    vec.y = displaySize.y;

  if (side & WINDOW_BORDER_NORTH) {
    newPos.y = vec.y;
    if (newPos.y < 0.0f)
      newPos.y = 0.0f;
    size.y -= newPos.y - pos.y;
  }
  else if (side & WINDOW_BORDER_SOUTH) {
    size.y = vec.y - newPos.y;
  }
  if (side & WINDOW_BORDER_WEST) {
    newPos.x = vec.x;
    if (newPos.x < 0.0f)
      newPos.x = 0.0f;
    size.x -= newPos.x - pos.x;
  }
  else if (side & WINDOW_BORDER_EAST) {
    size.x = vec.x - newPos.x;
  }

  pos = newPos;
  if (size.x < 5.0f)
    size.x = 5.0f;
  if (size.y < 5.0f)
    size.y = 5.0f;
  if (pos.x + size.x > displaySize.x) {
    size.x = displaySize.x - pos.x;
  }
  if (pos.y + size.y > displaySize.y) {
    size.y = displaySize.y - pos.y;
  }
}

void        HephGuiContext::callbackKey(int key, int scancode, int action, int mods) {
  if (getFocusedWindowPtr() != nullptr && getFocusedWindowPtr()->callbackKey != nullptr)
    getFocusedWindowPtr()->callbackKey(getFocusedWindowPtr(), key, scancode, action, mods);
}

void        HephGuiContext::callbackCharMods(uint32_t codepoint, int mods) {
  if (getFocusedWindowPtr() != nullptr && getFocusedWindowPtr()->callbackKey != nullptr)
    getFocusedWindowPtr()->callbackCharMods(getFocusedWindowPtr(), codepoint, mods);
}

void        HephGuiContext::callbackCursor(double x_pos, double y_pos) {
  cursor.pos = glm::vec2(x_pos, y_pos);
  for (int w = 0; w < winList.size(); w++) {
    if (x_pos >= winList[w].pos.x && y_pos >= winList[w].pos.y
        && x_pos < winList[w].pos.x + winList[w].size.x && y_pos < winList[w].pos.y + winList[w].size.y) {
      focusedWindowUUID = w;
    }
  }
  if (cursor.drag && cursor.resizeWin) {
    cursor.resizeWin->resizeFromCursor(cursor.side, cursor.pos);
    cursor.dragLast = cursor.pos;
    return ;
  }
  updateCursor();
  cursor.dragLast = glm::vec2(x_pos, y_pos);
  if (getFocusedWindowPtr() != nullptr && getFocusedWindowPtr()->callbackCursor != nullptr)
    getFocusedWindowPtr()->callbackCursor(getFocusedWindowPtr(), x_pos, y_pos);
}

void        HephGuiContext::callbackMouseButton(double x_pos, double y_pos, int button, int action, int mod) {
  cursor.pos = glm::ivec2(x_pos, y_pos);
  if (cursor.resizeWin != nullptr) {
    if (button == GLFW_MOUSE_BUTTON_1) {
      if (action == GLFW_PRESS) {
        cursor.dragLast = glm::vec2(x_pos, y_pos);
        cursor.drag = true;
        return ;
      }
    }
  }
  cursor.drag = false;
  if (getFocusedWindowPtr() != nullptr && getFocusedWindowPtr()->callbackMouseButton != nullptr)
    getFocusedWindowPtr()->callbackMouseButton(getFocusedWindowPtr(), x_pos, y_pos, button, action, mod);
}

void        HephGuiContext::callbackScroll(double x_pos, double y_pos, double xoffset, double yoffset) {
  if (getFocusedWindowPtr() != nullptr && getFocusedWindowPtr()->callbackScroll != nullptr)
    getFocusedWindowPtr()->callbackScroll(getFocusedWindowPtr(), x_pos, y_pos, xoffset, yoffset);
}

void        HephGuiContext::render(VkCommandBuffer commandBuffer) {
  uint32_t  totalVtx = 0;
  uint32_t  totalIdx = 0;

  for (auto& drawList: drawListBuffer) {
    totalVtx += drawList.vtxBuffer.size();
    totalIdx += drawList.idxBuffer.size();
  }

  if (totalVtx == 0 || totalIdx == 0)
    return ;

  if (vertexBuffer.size < totalVtx * sizeof(HephVertex)) {
    HephBufferCreateInfo  bufInfo = {
      .size = totalVtx * sizeof(HephVertex),
      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    };
    vkQueueWaitIdle(device.queues[0].queue);
    memoryAllocator.destroyBuffer(vertexBuffer);
    memoryAllocator.createBuffer(bufInfo, vertexBuffer);
  }
  if (indexBuffer.size < totalIdx * sizeof(uint32_t)) {
    HephBufferCreateInfo  bufInfo = {
      .size = totalIdx * sizeof(uint32_t),
      .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    };
    memoryAllocator.destroyBuffer(indexBuffer);
    memoryAllocator.createBuffer(bufInfo, indexBuffer);
  }
  HephVertex* vtxData = nullptr;
  uint32_t*   idxData = nullptr;
  vkMapMemory(device.device, vertexBuffer.memory, 0, vertexBuffer.size, 0, (void**)&vtxData);
  vkMapMemory(device.device, indexBuffer.memory, 0, indexBuffer.size, 0, (void**)&idxData);
  for (auto& drawList: drawListBuffer) {
    memcpy(vtxData, drawList.vtxBuffer.data(), drawList.vtxBuffer.size() * sizeof(HephVertex));
    memcpy(idxData, drawList.idxBuffer.data(), drawList.idxBuffer.size() * sizeof(uint32_t));
    vtxData += drawList.vtxBuffer.size();
    idxData += drawList.idxBuffer.size();
  }
  VkMappedMemoryRange range[2] = {};
  range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  range[0].memory = vertexBuffer.memory;
  range[0].size = VK_WHOLE_SIZE;
  range[1].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  range[1].memory = indexBuffer.memory;
  range[1].size = VK_WHOLE_SIZE;
  HEPH_PRINT_RESULT(vkFlushMappedMemoryRanges(device.device, 2, range));
  vkUnmapMemory(device.device, vertexBuffer.memory);
  vkUnmapMemory(device.device, indexBuffer.memory);

  VkDeviceSize offsets[1] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.buffer, offsets);
  vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

  VkViewport viewport = {
    .x = 0,
    .y = 0,
    .width = (float)displaySize.x,
    .height = (float)displaySize.y,
    .minDepth = 0.0f,
    .maxDepth = 1.0f,
  };
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  glm::vec2 scale = 2.0f / glm::vec2(displaySize);
  glm::vec2 translate = glm::vec2(-1.0, -1.0);

  vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::vec2), &scale);
  vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::vec2), sizeof(glm::vec2), &translate);

  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

  HephPipelineDescriptor* lastDescSet = nullptr;
  uint32_t                globalVtxOffset = 0;
  uint32_t                globalIdxOffset = 0;
  for (auto& drawList: drawListBuffer) {
    for (int i = drawList.drawCmdBuffer.size() - 1; i >= 0; i--) {
      auto&     drawCmd = drawList.drawCmdBuffer[i];
      VkRect2D  scissor;
      scissor.offset.x = (int32_t)((drawCmd.clipRect.x) < 0 ? 0 : (drawCmd.clipRect.x));
      scissor.offset.y = (int32_t)((drawCmd.clipRect.y) < 0 ? 0 : (drawCmd.clipRect.y));
      scissor.extent.width = (uint32_t)(drawCmd.clipRect.z - drawCmd.clipRect.x);
      scissor.extent.height = (uint32_t)(drawCmd.clipRect.w - drawCmd.clipRect.y);
      vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      HephPipelineDescriptor* descSet = &pipelineDescriptor;
      if (descSet != lastDescSet) {
        auto setBuffer = descSet->getSetBuffer(); 
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, setBuffer.size(), setBuffer.data(), 0, nullptr);
      }
      lastDescSet = descSet;

      vkCmdDrawIndexed(commandBuffer, drawCmd.elemCount, 1, drawCmd.idxOffset + globalIdxOffset, drawCmd.vtxOffset + globalVtxOffset, 0);
    }
    globalVtxOffset += drawList.vtxBuffer.size();
    globalIdxOffset += drawList.idxBuffer.size();
  }
}

}
