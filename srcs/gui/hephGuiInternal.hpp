#pragma once
#include "../core/hephaestus_core.hpp"
#include "../command/hephCommandPool.hpp"
#include "../memory/hephMemoryAllocator.hpp"
#include "../plugins/font/hephFont.hpp"
#include <glm/glm.hpp>
#include <stack>

namespace HephGui {

struct  HephVertex {
  glm::vec2 pos;
  glm::vec2 uv = glm::vec2(0, 0);
  glm::vec4 color;
};

struct  HephDrawCmd {
  glm::vec4   clipRect;
  uint32_t    idxOffset = 0;
  uint32_t    vtxOffset = 0;
  uint32_t    elemCount = 0;
};

struct  HephSharedData {
  glm::ivec4  clipRectFullScreen;
  HephFont*   font;
};

struct  HephDrawList {
  HephDrawList(HephSharedData* sharedData): _Data(sharedData) {}
  ~HephDrawList() {}
  std::vector<uint32_t>     idxBuffer;
  std::vector<HephVertex>   vtxBuffer;
  std::vector<HephDrawCmd>  drawCmdBuffer;

  void  addLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness = 1.0);
  void  addRect(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, float thickness = 1.0);
  void  addRectFill(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color);
  void  addPolyline(const glm::vec2* points, uint32_t size, const glm::vec4& color, float thickness = 1.0);

  void  addText(const std::string& text, float size, const glm::vec4& clipRect, const glm::vec4& color);

  inline float  addGlyphRect(const HephFont* font, const glm::vec2& pos, int c, float scale, const glm::vec4& color);

  //Internal
  std::vector<glm::vec2>    _Path;
  std::stack<glm::vec4>     _ClipRectStack;
  HephSharedData*           _Data;

  void  pushClipRect(const glm::ivec2& clipRect_min, const glm::ivec2& clipRect_max, bool intersect_with_current_clip_rect = false);
  void  pushClipRectFullScreen();
  void  popClipRect();
  //void  pushTexture(ImTextureRef tex_ref);
  //void  popTexture();
  inline glm::vec2  getClipRectMin() const {const glm::ivec4& cr = _ClipRectStack.top(); return glm::vec2(cr.x, cr.y);}
  inline glm::vec2  getClipRectMax() const {const glm::ivec4& cr = _ClipRectStack.top(); return glm::vec2(cr.z, cr.w);}

  inline void   pathClear() {_Path.resize(0);}
  inline void   pathLineTo(const glm::vec2& p) {_Path.push_back(p);}
  inline void   pathStroke(glm::vec4 color, float thickness = 1.0f) {addPolyline(_Path.data(), _Path.size(), color, thickness); _Path.resize(0);}

};

class   HephGuiContext {
  public:
    HephGuiContext() {};

    HephResult          createPipeline();
    void                destroy();
    void                render(VkCommandBuffer cmdBuffer);
    void                newFrame() {drawListBuffer.clear();}
    HephSharedData*     getSharedData() {return (&sharedData);};

    void                setDisplaySize(int width, int height) {
      displaySize.x = width;
      displaySize.y = height;
      sharedData.clipRectFullScreen = glm::ivec4(displayPos.x, displayPos.y, displayPos.x + width, displayPos.y + height);
    }

    glm::ivec2                displayPos = glm::ivec2(0, 0);
    glm::ivec2                displaySize;

    HephSharedData            sharedData;

    std::vector<HephDrawList> drawListBuffer;

    HephDevice                device;
    VkRenderPass              renderPass = VK_NULL_HANDLE;
		HephPipelineDescriptor	  pipelineDescriptor;
		VkPipelineLayout          pipelineLayout = VK_NULL_HANDLE;
		VkPipeline          		  pipeline = VK_NULL_HANDLE;
		HephCommandPool			      commandPool;
		HephMemoryAllocator	      memoryAllocator; 

    HephFont                  font;

    HephBufferWrapper         vertexBuffer;
    HephBufferWrapper         indexBuffer;
};

}
