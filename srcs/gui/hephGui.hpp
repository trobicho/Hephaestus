#pragma once
#include "../core/hephaestus_core.hpp"
#include "../command/hephCommandPool.hpp"
#include "../memory/hephMemoryAllocator.hpp"
#include "../plugins/font/hephFont.hpp"
#include <glm/glm.hpp>

namespace HephGui {

struct  HephGuiVertex {
  glm::vec2 pos;
  glm::vec2 uv;
  glm::vec4 color;
};

struct  HephDrawList {
  std::vector<uint32_t>       idxBuffer;
  std::vector<HephGuiVertex>  vtxBuffer;

  //Internal
  std::vector<glm::vec2>      _Path;

  void  addLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& col, float thickness = 1.0);
  void  addRect(const glm::vec2& min, const glm::vec2& max, const glm::vec4& col, float thickness = 1.0);
  void  addRectFill(const glm::vec2& min, const glm::vec2& max, const glm::vec4& col);
  void  addPolyLine(const glm::vec2* points, uint32_t size, const glm::vec4& color, float thickness = 1.0);

  inline void  pathLineTo(const glm::vec2& p) {_Path.push_back(p);}
  inline void  pathStroke(glm::vec4 color, float thickness = 1.0f) {addPolyline(_Path.data(), _Path.size(), col, thickness); _Path.resize(0);}
};

class   HephGuiContext {
  public:
    HephGuiContext() {};

    static HephResult   init();
    HephResult          create(HephDevice& device, VkRenderPass renderPass = VK_NULL_HANDLE);

  private:
    HephResult          createPipelines();

    HephDevice              m_device;
    VkRenderPass            m_renderPass = VK_NULL_HANDLE;
		HephPipelineDescriptor	m_pipelineDescriptor;
		VkPipelineLayout        m_pipelineLayout = VK_NULL_HANDLE;
		VkPipeline          		m_pipelineLine = VK_NULL_HANDLE;
		VkPipeline          		m_pipelineTri = VK_NULL_HANDLE;
		HephCommandPool			    m_commandPool;
		HephMemoryAllocator	    m_memoryAllocator; 
};

}
