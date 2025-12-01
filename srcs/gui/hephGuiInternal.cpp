#include "hephGuiInternal.hpp"

namespace HephGui {

void        HephGuiContext::destroy() {
  memoryAllocator.destroyBuffer(vertexBuffer);
  memoryAllocator.destroyBuffer(indexBuffer);
  commandPool.destroy();
  vkDestroyPipeline(device.device, pipeline, device.pAllocationCallbacks);
  vkDestroyPipelineLayout(device.device, pipelineLayout, device.pAllocationCallbacks);
  pipeline = VK_NULL_HANDLE;
  pipelineLayout = VK_NULL_HANDLE;
  pipelineDescriptor.destroy(device);
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
    memoryAllocator.createBuffer(bufInfo, vertexBuffer);
  }
  if (indexBuffer.size < totalIdx * sizeof(uint32_t)) {
    HephBufferCreateInfo  bufInfo = {
      .size = totalIdx * sizeof(uint32_t),
      .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    };
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
    for (auto& drawCmd: drawList.drawCmdBuffer) {
      VkRect2D scissor;
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
