#pragma once
#include <atomic>
#include "hephaestus.hpp"
#include "gui/hephGui.hpp"
#include "gui/hephGuiInternal.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

class GuiTest: public HephGui::HephApp {
  public:
    GuiTest() {}
    ~GuiTest() {destroy();}

    void  destroy() override {};
    void  initWindow() override;
    void  render() override;

    void  callbackKey(HephGui::HephWindow* window, int key, int scancode, int action, int mods);
    void  callbackCursor(HephGui::HephWindow* window, glm::vec2 pos);
    void  callbackMouseButton(HephGui::HephWindow* window, int button, int action, int mod);
    void  callbackScroll(HephGui::HephWindow* window, double xoffset, double yoffset);
    void  callbackResize(HephGui::HephWindow* window, glm::ivec2 pos, glm::ivec2 size);
    void  callbackDrawListRender(HephGui::HephDrawList* drawList, VkCommandBuffer commandBuffer);

    void  reset() {}


  private:
    void        updateDescriptorSets();

    HephGui::HephDrawListDirectToGpu  m_drawList;
    HephBufferWrapper                 m_vertexBuffer;
    HephBufferWrapper                 m_indexBuffer;
    std::atomic<bool>                 m_needUpdate{true};
    std::atomic<bool>                 m_needAlloc{false};

    std::string                       m_textValue = "salut";

    int                               m_sliderIntV1 = 1;
    int                               m_sliderIntV2 = 10;
    int                               m_sliderIntV3 = 0;
    int                               m_sliderIntV4 = 10;

    float                             m_sliderFloatV1 = 1.0f;
    float                             m_sliderFloatV2 = 10.0f;
    float                             m_sliderFloatV3 = 0.3f;
    float                             m_sliderFloatV4 = 0.0001f;
};
