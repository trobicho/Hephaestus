#pragma once
#include "../core/hephaestus_core.hpp"
#include "../command/hephCommandPool.hpp"
#include "../memory/hephMemoryAllocator.hpp"
#include "../plugins/font/hephFont.hpp"
#include "../texture/hephTexture.hpp"
#include "hephGui.hpp"
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <stack>
#include <vulkan/vulkan_core.h>

#define   WINDOW_BORDER_NORTH   1
#define   WINDOW_BORDER_SOUTH   2
#define   WINDOW_BORDER_WEST    4
#define   WINDOW_BORDER_EAST    8

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
  glm::ivec4      clipRectFullScreen;
  HephTextureArea whitePixelArea;
  HephFont*       font;
};

struct  HephDrawList {
  HephDrawList(HephSharedData* sharedData): _Data(sharedData) {}
  ~HephDrawList() {}
  std::vector<uint32_t>     idxBuffer;
  std::vector<HephVertex>   vtxBuffer;
  std::vector<HephDrawCmd>  drawCmdBuffer;

  void  addDrawCmd() {
    drawCmdBuffer.push_back(HephDrawCmd());
    drawCmdBuffer.back().idxOffset = idxBuffer.size();
    assert(!_ClipRectStack.empty() && "clipRectStack cannot be empty when adding drawCmd");
    drawCmdBuffer.back().clipRect = _ClipRectStack.top();
  }

  void          addLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness = 1.0);
  void          addRect(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, float thickness = 1.0);
  void          addCircle(const glm::vec2& center, float radius, int steps, const glm::vec4& color, float thickness = 1.0);
  void          addEllipsis(const glm::vec2& center, float r1, float r2, float angle, int steps, const glm::vec4& color, float thickness = 1.0f);
  void          addEllipsis(const glm::vec2& center, float r1, float r2, float angle
                  , int pertNum, float pertAmp, int steps, const glm::vec4& color, float thickness);
  inline void   addRectFill(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color) {
    addRectFill(min, max, color, _Data->whitePixelArea);
  }
  void          addRectFill(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, const HephTextureArea& area);
  void          addPolyline(const glm::vec2* points, uint32_t size, const glm::vec4& color, float thickness = 1.0);

  void          addText(const std::string& text, float size, const glm::vec4& clipRect, const glm::vec4& color);
  void          addText(const char* text, float size, const glm::vec4& clipRect, const glm::vec4& color);
  void          addText(const char* text, int textLen, float size, const glm::vec4& clipRect, const glm::vec4& color);
  glm::vec2     getTextSize(const std::string& text, float size);

  inline float  addGlyphRect(const HephFont* font, const HephFontFace& face, const glm::vec2& pos, int c, float size, const glm::vec4& color);

  //Internal
  std::vector<glm::vec2>    _Path;
  std::stack<glm::ivec4>    _ClipRectStack;
  HephSharedData*           _Data;

  void                      (*userDrawListRender)(HephDrawList*, VkCommandBuffer) = nullptr;
  void*                     userPtr = nullptr;

  void  pushClipRect(const glm::ivec2& clipRect_min, const glm::ivec2& clipRect_max, bool intersect_with_current_clip_rect = false);
  void  pushClipRectFullScreen();
  void  popClipRect(bool letEmpty = false);
  //void  pushTexture(ImTextureRef tex_ref);
  //void  popTexture();
  inline glm::ivec2 getClipRectMin() const {const glm::ivec4& cr = _ClipRectStack.top(); return glm::ivec2(cr.x, cr.y);}
  inline glm::ivec2 getClipRectMax() const {const glm::ivec4& cr = _ClipRectStack.top(); return glm::ivec2(cr.z, cr.w);}
  inline glm::ivec4 getClipRect() const {return (_ClipRectStack.top());}

  inline void   pathClear() {_Path.resize(0);}
  inline void   pathLineTo(const glm::vec2& p) {_Path.push_back(p);}
  inline void   pathStroke(glm::vec4 color, float thickness = 1.0f) {addPolyline(_Path.data(), _Path.size(), color, thickness); _Path.resize(0);}

  inline virtual void      idxBufferAdd(const uint32_t& idx) {idxBuffer.push_back(idx);}
  inline virtual void      vtxBufferAdd(const HephVertex& vtx) {vtxBuffer.push_back(vtx);}
  inline virtual uint32_t  idxBufferCount() const {return (idxBuffer.size());}
  inline virtual uint32_t  vtxBufferCount() const {return (vtxBuffer.size());}
};

struct  HephDrawListDirectToGpu: public HephDrawList {
  HephDrawListDirectToGpu(): HephDrawList(nullptr) {}
  ~HephDrawListDirectToGpu() {}

  HephVertex*       vtxData = nullptr;
  uint32_t*         idxData = nullptr;
  uint32_t          vtxCount = 0;
  uint32_t          idxCount = 0;
  HephBufferWrapper vertexBuffer;
  HephBufferWrapper indexBuffer;

  inline virtual  void      idxBufferAdd(const uint32_t& idx) override {
    if (idxData != nullptr)
      idxData[idxCount++] = idx;
    else
      idxBuffer.push_back(idx);
  }
  inline virtual  void      vtxBufferAdd(const HephVertex& vtx) override {
    if (vtxData != nullptr)
      vtxData[vtxCount++] = vtx;
    else
      vtxBuffer.push_back(vtx);
  }
  inline virtual  uint32_t  idxBufferCount() const override {
    if (idxData != nullptr)
      return (idxCount);
    return (idxBuffer.size());
  }
  inline virtual  uint32_t  vtxBufferCount() const override {
    if (vtxData != nullptr)
      return (vtxCount);
    return (vtxBuffer.size());
  }

  void                      allocateAndWriteToBuffer();

  void                      reset() {
    vtxData = nullptr;
    idxData = nullptr;
    vtxCount = 0;
    idxCount = 0;
    vtxBuffer.resize(0);
    idxBuffer.resize(0);
    drawCmdBuffer.resize(0);
    addDrawCmd();
  }

  void                      newFrame() {
    pathClear();
    while (!_ClipRectStack.empty())
      _ClipRectStack.pop();
  }
};

struct  HephGuiStyle {
  glm::vec4 colors[HephGuiCol_::HephGuiCol_COUNT] = {glm::vec4(1.0)};
};

struct  HephGuiStyleMod {
  int       idx;
  glm::vec4 color;
};

class   HephGuiLayout {
  public:
    virtual void    close(HephGuiLayout* ptr) = 0;
    virtual void    resize(HephGuiLayout* ptr, glm::ivec2 a_pos, glm::ivec2 a_size) = 0;
    virtual void    resizeInternal(glm::ivec2 a_pos, glm::ivec2 a_size) = 0;
    virtual void    ptrChildChange(HephGuiLayout* ptr, HephGuiLayout* newPtr) {};

    glm::ivec2      pos = glm::ivec2(0, 0);
    glm::ivec2      size = glm::ivec2(-1, -1);
    HephGuiLayout*  parentLayout = nullptr;
};

struct  HephWindowFrameData {
  void        newFrame(glm::ivec2 winPos) {
    cursorStartPos = winPos + 5;
    cursorPos = cursorStartPos;
    cursorMaxPos = cursorPos;
    isSameLine = false;
  }

  glm::ivec2  cursorPos;
  glm::ivec2  cursorStartPos;
  glm::ivec2  cursorMaxPos;

  bool        isSameLine;
};

struct  HephWindowItem {
  HephWindowItem() = default;
  HephWindowItem(HephGuiId id, const glm::vec4& bb): id(id), bb(bb) {}

  HephGuiId   id;
  glm::ivec4  bb;
};

struct  HephWindow: public HephGuiLayout {
  HephWindow(std::string name): name(name) {};

  void        newFrame();

  void  (*callbackKey)(HephWindow* window, int key, int scancode, int action, int mods) = nullptr;
  void  (*callbackCharMods)(HephWindow* window, uint32_t codepoint, int mods)           = nullptr;
  void  (*callbackCursorPos)(HephWindow* window, glm::vec2 pos)                         = nullptr;
  void  (*callbackMouseButton)(HephWindow* window, int button, int action, int mod)     = nullptr;
  void  (*callbackScroll)(HephWindow* window, double xoffset, double yoffset)           = nullptr;
  void  (*callbackDrop)(HephWindow* window, int count, const char** paths)              = nullptr;
  void  (*callbackResize)(HephWindow* window, glm::ivec2 pos, glm::ivec2 size)          = nullptr;
  void  (*callbackClose)(HephWindow* window)                                            = nullptr;

  void              resizeFromCursor(int side, glm::ivec2 vec);

  virtual HephGuiId itemAdd(const glm::ivec4& bb, HephGuiId id);

  virtual void      close(HephGuiLayout* ptr) override {
    if (parentLayout != nullptr && ptr != parentLayout) {
      parentLayout->close(this);
    }
    if (callbackClose) 
      callbackClose(this);
  }
  virtual void      resize(HephGuiLayout* ptr, glm::ivec2 a_pos, glm::ivec2 a_size) override {
    if (parentLayout != nullptr && ptr != parentLayout) {
      parentLayout->resize(this, a_pos, a_size);
    }
    else
      resizeInternal(a_pos, a_size);
    drawList->popClipRect(true);
    drawList->pushClipRect(pos, pos + size);
  }
  virtual void      resizeInternal(glm::ivec2 a_pos, glm::ivec2 a_size) override {
    if (a_pos == pos && a_size == size)
      return ;
    if (callbackResize) {
      callbackResize(this, a_pos, a_size);
    }
    pos = a_pos;
    size = a_size;
  }

  std::string                 name;
  bool                        firstFrame = true;

  HephWindowFrameData         frameData;
  HephDrawList*               drawList = nullptr;
  void*                       userPtr = nullptr;
  std::vector<HephWindowItem> itemList;
};

struct  HephGuiInputTextState {
  HephGuiContext*   ctx;
  HephGuiId         id;
  std::vector<char> text;
  std::vector<char> textToRevertTo;
  char*             textSrc;
  int               textLen;

  bool              editedBefore = false;     // edited since activated
  bool              editedThisFrame = false;  // edited this frame
  int               cursorPos = 0;
};

enum    HephGuiCursorType {
  HephGuiCursor_Arrow,
  HephGuiCursor_TextInput,
  HephGuiCursor_ResizeNS,
  HephGuiCursor_ResizeEW,
  HephGuiCursor_ResizeNESW,
  HephGuiCursor_ResizeNWSE,
  HephGuiCursor_ResizeAll,
  HephGuiCursor_Hand,
  HephGuiCursor_NotAllowed,

  HephGuiCursor_COUNT,
};

struct  HephGuiCursor {
  HephGuiCursorType type = HephGuiCursor_Arrow;
  glm::vec2         pos = glm::vec2(0, 0);
  HephWindow*       resizeWin = nullptr;
  bool              drag = false;
  glm::vec2         dragLast;
  int               side = 0;
};

class   HephGuiContext {
  public:
    HephGuiContext() {};
    ~HephGuiContext() {
      for (auto& winPtr: winList) {
        delete (winPtr);
      }
    };

    HephResult          create(HephGuiCreateInfo& createInfo);
    HephResult          createPipeline();
    void                updateDescriptorSets();
    void                destroy();
    void                render(VkCommandBuffer cmdBuffer);
    void                newFrame() {
      userDrawListBuffer.clear();
      drawListBuffer.clear();
    }
    HephSharedData*     getSharedData() {return (&sharedData);};
    inline HephWindow*  getFocusedWindowPtr() {if (focusedWindowUUID >= 0) {return (winList[focusedWindowUUID]);} return (nullptr);}
    void                updateCursor();

    void                setDisplaySize(int width, int height) {
      displaySize.x = width;
      displaySize.y = height;
      sharedData.clipRectFullScreen = glm::ivec4(displayPos.x, displayPos.y, displayPos.x + width, displayPos.y + height);
    }

    HephGuiInputTextState*  getInputTextState(const HephGuiId& id) {
      if (inputTextState.id == id)
        return (&inputTextState);
      return (nullptr);
    }

    //EVENT CALLBACKS
    void  callbackKey(int key, int scancode, int action, int mods);
    void  callbackCharMods(uint32_t codepoint, int mods);
    void  callbackCursorPos(glm::vec2 pos);
    void  callbackMouseButton(int button, int action, int mod);
    void  callbackScroll(double xoffset, double yoffset);
    void  callbackDrop(int count, const char** paths);
    //===============

    glm::ivec2                  displayPos = glm::ivec2(0, 0);
    glm::ivec2                  displaySize;

    HephSharedData              sharedData;

    HephGuiCursor               cursor;
    HephGuiStyle                style;

    std::vector<HephDrawList*>  userDrawListBuffer;
    std::vector<HephDrawList>   drawListBuffer;
    std::vector<HephWindow*>    winList;

    int                         focusedWindowUUID = -1;

    HephGuiId                   activeId;
    HephGuiId                   hoveredId;
    bool                        activeIdIsJustActivated = false;
    bool                        activeIdIsHeld = false;
    HephGuiInputTextState       inputTextState;

    HephDevice                        device;
    VkRenderPass                      renderPass = VK_NULL_HANDLE;
    VkPipelineRenderingCreateInfoKHR  pipelineRenderingInfo;
    bool                              dynamicRendingEnable = false;
		HephPipelineDescriptor	          pipelineDescriptor;
		VkPipelineLayout                  pipelineLayout = VK_NULL_HANDLE;
		VkPipeline          		          pipeline = VK_NULL_HANDLE;
		HephCommandPool			              commandPool;
		HephMemoryAllocator	              memoryAllocator; 
    VkCommandBuffer                   currentCommandBuffer = VK_NULL_HANDLE;

    HephFont                    font;
    VkSampler                   sampler;

    HephBufferWrapper           vertexBuffer;
    HephBufferWrapper           indexBuffer;
};

}
