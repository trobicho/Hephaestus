#include "gui/hephGuiInternal.hpp"
#include "hephGui.hpp"
#include "plugins/font/hephFont.hpp"
#include "texture/hephTexture.hpp"
#include <cstdint>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

namespace HephGui {

void StyleColorsDark(HephGuiStyle* dst)
{
    HephGuiStyle* style = dst ? dst : &HephGui::GetStyle();
    glm::vec4*    colors = style->colors;

    colors[HephGuiCol_Text]                   = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[HephGuiCol_TextDisabled]           = glm::vec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[HephGuiCol_WindowBg]               = glm::vec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[HephGuiCol_ChildBg]                = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_PopupBg]                = glm::vec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[HephGuiCol_Border]                 = glm::vec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[HephGuiCol_BorderShadow]           = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_FrameBg]                = glm::vec4(0.16f, 0.29f, 0.48f, 0.54f);
    colors[HephGuiCol_FrameBgHovered]         = glm::vec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[HephGuiCol_FrameBgActive]          = glm::vec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[HephGuiCol_TitleBg]                = glm::vec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[HephGuiCol_TitleBgActive]          = glm::vec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[HephGuiCol_TitleBgCollapsed]       = glm::vec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[HephGuiCol_MenuBarBg]              = glm::vec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[HephGuiCol_ScrollbarBg]            = glm::vec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[HephGuiCol_ScrollbarGrab]          = glm::vec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[HephGuiCol_ScrollbarGrabHovered]   = glm::vec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[HephGuiCol_ScrollbarGrabActive]    = glm::vec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[HephGuiCol_CheckMark]              = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_SliderGrab]             = glm::vec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[HephGuiCol_SliderGrabActive]       = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_Button]                 = glm::vec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[HephGuiCol_ButtonHovered]          = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_ButtonActive]           = glm::vec4(0.06f, 0.53f, 0.98f, 1.00f);

    /*
    colors[HephGuiCol_Header]                 = glm::vec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[HephGuiCol_HeaderHovered]          = glm::vec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[HephGuiCol_HeaderActive]           = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_Separator]              = colors[HephGuiCol_Border];
    colors[HephGuiCol_SeparatorHovered]       = glm::vec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[HephGuiCol_SeparatorActive]        = glm::vec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[HephGuiCol_ResizeGrip]             = glm::vec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[HephGuiCol_ResizeGripHovered]      = glm::vec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[HephGuiCol_ResizeGripActive]       = glm::vec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[HephGuiCol_InputTextCursor]        = colors[HephGuiCol_Text];
    colors[HephGuiCol_TabHovered]             = colors[HephGuiCol_HeaderHovered];
    colors[HephGuiCol_Tab]                    = ImLerp(colors[HephGuiCol_Header],       colors[HephGuiCol_TitleBgActive], 0.80f);
    colors[HephGuiCol_TabSelected]            = ImLerp(colors[HephGuiCol_HeaderActive], colors[HephGuiCol_TitleBgActive], 0.60f);
    colors[HephGuiCol_TabSelectedOverline]    = colors[HephGuiCol_HeaderActive];
    colors[HephGuiCol_TabDimmed]              = ImLerp(colors[HephGuiCol_Tab],          colors[HephGuiCol_TitleBg], 0.80f);
    colors[HephGuiCol_TabDimmedSelected]      = ImLerp(colors[HephGuiCol_TabSelected],  colors[HephGuiCol_TitleBg], 0.40f);
    colors[HephGuiCol_TabDimmedSelectedOverline] = glm::vec4(0.50f, 0.50f, 0.50f, 0.00f);
    colors[HephGuiCol_PlotLines]              = glm::vec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[HephGuiCol_PlotLinesHovered]       = glm::vec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[HephGuiCol_PlotHistogram]          = glm::vec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[HephGuiCol_PlotHistogramHovered]   = glm::vec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[HephGuiCol_TableHeaderBg]          = glm::vec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[HephGuiCol_TableBorderStrong]      = glm::vec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[HephGuiCol_TableBorderLight]       = glm::vec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[HephGuiCol_TableRowBg]             = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_TableRowBgAlt]          = glm::vec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[HephGuiCol_TextLink]               = colors[HephGuiCol_HeaderActive];
    colors[HephGuiCol_TextSelectedBg]         = glm::vec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[HephGuiCol_TreeLines]              = colors[HephGuiCol_Border];
    colors[HephGuiCol_DragDropTarget]         = glm::vec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[HephGuiCol_DragDropTargetBg]       = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_UnsavedMarker]          = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[HephGuiCol_NavCursor]              = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_NavWindowingHighlight]  = glm::vec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[HephGuiCol_NavWindowingDimBg]      = glm::vec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[HephGuiCol_ModalWindowDimBg]       = glm::vec4(0.80f, 0.80f, 0.80f, 0.35f);
    */
}

void StyleColorsClassic(HephGuiStyle* dst)
{
    HephGuiStyle* style = dst ? dst : &HephGui::GetStyle();
    glm::vec4*    colors = style->colors;

    colors[HephGuiCol_Text]                   = glm::vec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[HephGuiCol_TextDisabled]           = glm::vec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[HephGuiCol_WindowBg]               = glm::vec4(0.04f, 0.04f, 0.04f, 0.80f);
    colors[HephGuiCol_ChildBg]                = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_PopupBg]                = glm::vec4(0.11f, 0.11f, 0.14f, 0.92f);
    colors[HephGuiCol_Border]                 = glm::vec4(0.50f, 0.50f, 0.50f, 0.50f);
    colors[HephGuiCol_BorderShadow]           = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_FrameBg]                = glm::vec4(0.43f, 0.43f, 0.43f, 0.39f);
    colors[HephGuiCol_FrameBgHovered]         = glm::vec4(0.47f, 0.47f, 0.69f, 0.40f);
    colors[HephGuiCol_FrameBgActive]          = glm::vec4(0.42f, 0.41f, 0.64f, 0.69f);
    colors[HephGuiCol_TitleBg]                = glm::vec4(0.27f, 0.27f, 0.54f, 0.83f);
    colors[HephGuiCol_TitleBgActive]          = glm::vec4(0.32f, 0.32f, 0.63f, 0.87f);
    colors[HephGuiCol_TitleBgCollapsed]       = glm::vec4(0.40f, 0.40f, 0.80f, 0.20f);
    colors[HephGuiCol_MenuBarBg]              = glm::vec4(0.40f, 0.40f, 0.55f, 0.80f);
    colors[HephGuiCol_ScrollbarBg]            = glm::vec4(0.20f, 0.25f, 0.30f, 0.60f);
    colors[HephGuiCol_ScrollbarGrab]          = glm::vec4(0.40f, 0.40f, 0.80f, 0.30f);
    colors[HephGuiCol_ScrollbarGrabHovered]   = glm::vec4(0.40f, 0.40f, 0.80f, 0.40f);
    colors[HephGuiCol_ScrollbarGrabActive]    = glm::vec4(0.41f, 0.39f, 0.80f, 0.60f);
    colors[HephGuiCol_CheckMark]              = glm::vec4(0.90f, 0.90f, 0.90f, 0.50f);
    colors[HephGuiCol_SliderGrab]             = glm::vec4(1.00f, 1.00f, 1.00f, 0.30f);
    colors[HephGuiCol_SliderGrabActive]       = glm::vec4(0.41f, 0.39f, 0.80f, 0.60f);
    colors[HephGuiCol_Button]                 = glm::vec4(0.35f, 0.40f, 0.61f, 0.62f);
    colors[HephGuiCol_ButtonHovered]          = glm::vec4(0.40f, 0.48f, 0.71f, 0.79f);
    colors[HephGuiCol_ButtonActive]           = glm::vec4(0.46f, 0.54f, 0.80f, 1.00f);

    /*
    colors[HephGuiCol_Header]                 = glm::vec4(0.40f, 0.40f, 0.90f, 0.45f);
    colors[HephGuiCol_HeaderHovered]          = glm::vec4(0.45f, 0.45f, 0.90f, 0.80f);
    colors[HephGuiCol_HeaderActive]           = glm::vec4(0.53f, 0.53f, 0.87f, 0.80f);
    colors[HephGuiCol_Separator]              = glm::vec4(0.50f, 0.50f, 0.50f, 0.60f);
    colors[HephGuiCol_SeparatorHovered]       = glm::vec4(0.60f, 0.60f, 0.70f, 1.00f);
    colors[HephGuiCol_SeparatorActive]        = glm::vec4(0.70f, 0.70f, 0.90f, 1.00f);
    colors[HephGuiCol_ResizeGrip]             = glm::vec4(1.00f, 1.00f, 1.00f, 0.10f);
    colors[HephGuiCol_ResizeGripHovered]      = glm::vec4(0.78f, 0.82f, 1.00f, 0.60f);
    colors[HephGuiCol_ResizeGripActive]       = glm::vec4(0.78f, 0.82f, 1.00f, 0.90f);
    colors[HephGuiCol_InputTextCursor]        = colors[HephGuiCol_Text];
    colors[HephGuiCol_TabHovered]             = colors[HephGuiCol_HeaderHovered];
    colors[HephGuiCol_Tab]                    = ImLerp(colors[HephGuiCol_Header],       colors[HephGuiCol_TitleBgActive], 0.80f);
    colors[HephGuiCol_TabSelected]            = ImLerp(colors[HephGuiCol_HeaderActive], colors[HephGuiCol_TitleBgActive], 0.60f);
    colors[HephGuiCol_TabSelectedOverline]    = colors[HephGuiCol_HeaderActive];
    colors[HephGuiCol_TabDimmed]              = ImLerp(colors[HephGuiCol_Tab],          colors[HephGuiCol_TitleBg], 0.80f);
    colors[HephGuiCol_TabDimmedSelected]      = ImLerp(colors[HephGuiCol_TabSelected],  colors[HephGuiCol_TitleBg], 0.40f);
    colors[HephGuiCol_TabDimmedSelectedOverline] = glm::vec4(0.53f, 0.53f, 0.87f, 0.00f);
    colors[HephGuiCol_PlotLines]              = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[HephGuiCol_PlotLinesHovered]       = glm::vec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[HephGuiCol_PlotHistogram]          = glm::vec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[HephGuiCol_PlotHistogramHovered]   = glm::vec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[HephGuiCol_TableHeaderBg]          = glm::vec4(0.27f, 0.27f, 0.38f, 1.00f);
    colors[HephGuiCol_TableBorderStrong]      = glm::vec4(0.31f, 0.31f, 0.45f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[HephGuiCol_TableBorderLight]       = glm::vec4(0.26f, 0.26f, 0.28f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[HephGuiCol_TableRowBg]             = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_TableRowBgAlt]          = glm::vec4(1.00f, 1.00f, 1.00f, 0.07f);
    colors[HephGuiCol_TextLink]               = colors[HephGuiCol_HeaderActive];
    colors[HephGuiCol_TextSelectedBg]         = glm::vec4(0.00f, 0.00f, 1.00f, 0.35f);
    colors[HephGuiCol_TreeLines]              = colors[HephGuiCol_Border];
    colors[HephGuiCol_DragDropTarget]         = glm::vec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[HephGuiCol_DragDropTargetBg]       = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_UnsavedMarker]          = glm::vec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[HephGuiCol_NavCursor]              = colors[HephGuiCol_HeaderHovered];
    colors[HephGuiCol_NavWindowingHighlight]  = glm::vec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[HephGuiCol_NavWindowingDimBg]      = glm::vec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[HephGuiCol_ModalWindowDimBg]       = glm::vec4(0.20f, 0.20f, 0.20f, 0.35f);
    */
}

// Those light colors are better suited with a thicker font than the default one + FrameBorder
void StyleColorsLight(HephGuiStyle* dst)
{
    HephGuiStyle* style = dst ? dst : &HephGui::GetStyle();
    glm::vec4*    colors = style->colors;

    colors[HephGuiCol_Text]                   = glm::vec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[HephGuiCol_TextDisabled]           = glm::vec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[HephGuiCol_WindowBg]               = glm::vec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[HephGuiCol_ChildBg]                = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_PopupBg]                = glm::vec4(1.00f, 1.00f, 1.00f, 0.98f);
    colors[HephGuiCol_Border]                 = glm::vec4(0.00f, 0.00f, 0.00f, 0.30f);
    colors[HephGuiCol_BorderShadow]           = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_FrameBg]                = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[HephGuiCol_FrameBgHovered]         = glm::vec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[HephGuiCol_FrameBgActive]          = glm::vec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[HephGuiCol_TitleBg]                = glm::vec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[HephGuiCol_TitleBgActive]          = glm::vec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[HephGuiCol_TitleBgCollapsed]       = glm::vec4(1.00f, 1.00f, 1.00f, 0.51f);
    colors[HephGuiCol_MenuBarBg]              = glm::vec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[HephGuiCol_ScrollbarBg]            = glm::vec4(0.98f, 0.98f, 0.98f, 0.53f);
    colors[HephGuiCol_ScrollbarGrab]          = glm::vec4(0.69f, 0.69f, 0.69f, 0.80f);
    colors[HephGuiCol_ScrollbarGrabHovered]   = glm::vec4(0.49f, 0.49f, 0.49f, 0.80f);
    colors[HephGuiCol_ScrollbarGrabActive]    = glm::vec4(0.49f, 0.49f, 0.49f, 1.00f);
    colors[HephGuiCol_CheckMark]              = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_SliderGrab]             = glm::vec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[HephGuiCol_SliderGrabActive]       = glm::vec4(0.46f, 0.54f, 0.80f, 0.60f);
    colors[HephGuiCol_Button]                 = glm::vec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[HephGuiCol_ButtonHovered]          = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_ButtonActive]           = glm::vec4(0.06f, 0.53f, 0.98f, 1.00f);

    /*
    colors[HephGuiCol_Header]                 = glm::vec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[HephGuiCol_HeaderHovered]          = glm::vec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[HephGuiCol_HeaderActive]           = glm::vec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[HephGuiCol_Separator]              = glm::vec4(0.39f, 0.39f, 0.39f, 0.62f);
    colors[HephGuiCol_SeparatorHovered]       = glm::vec4(0.14f, 0.44f, 0.80f, 0.78f);
    colors[HephGuiCol_SeparatorActive]        = glm::vec4(0.14f, 0.44f, 0.80f, 1.00f);
    colors[HephGuiCol_ResizeGrip]             = glm::vec4(0.35f, 0.35f, 0.35f, 0.17f);
    colors[HephGuiCol_ResizeGripHovered]      = glm::vec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[HephGuiCol_ResizeGripActive]       = glm::vec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[HephGuiCol_InputTextCursor]        = colors[HephGuiCol_Text];
    colors[HephGuiCol_TabHovered]             = colors[HephGuiCol_HeaderHovered];
    colors[HephGuiCol_Tab]                    = ImLerp(colors[HephGuiCol_Header],       colors[HephGuiCol_TitleBgActive], 0.90f);
    colors[HephGuiCol_TabSelected]            = ImLerp(colors[HephGuiCol_HeaderActive], colors[HephGuiCol_TitleBgActive], 0.60f);
    colors[HephGuiCol_TabSelectedOverline]    = colors[HephGuiCol_HeaderActive];
    colors[HephGuiCol_TabDimmed]              = ImLerp(colors[HephGuiCol_Tab],          colors[HephGuiCol_TitleBg], 0.80f);
    colors[HephGuiCol_TabDimmedSelected]      = ImLerp(colors[HephGuiCol_TabSelected],  colors[HephGuiCol_TitleBg], 0.40f);
    colors[HephGuiCol_TabDimmedSelectedOverline] = glm::vec4(0.26f, 0.59f, 1.00f, 0.00f);
    colors[HephGuiCol_PlotLines]              = glm::vec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[HephGuiCol_PlotLinesHovered]       = glm::vec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[HephGuiCol_PlotHistogram]          = glm::vec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[HephGuiCol_PlotHistogramHovered]   = glm::vec4(1.00f, 0.45f, 0.00f, 1.00f);
    colors[HephGuiCol_TableHeaderBg]          = glm::vec4(0.78f, 0.87f, 0.98f, 1.00f);
    colors[HephGuiCol_TableBorderStrong]      = glm::vec4(0.57f, 0.57f, 0.64f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[HephGuiCol_TableBorderLight]       = glm::vec4(0.68f, 0.68f, 0.74f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[HephGuiCol_TableRowBg]             = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_TableRowBgAlt]          = glm::vec4(0.30f, 0.30f, 0.30f, 0.09f);
    colors[HephGuiCol_TextLink]               = colors[HephGuiCol_HeaderActive];
    colors[HephGuiCol_TextSelectedBg]         = glm::vec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[HephGuiCol_TreeLines]              = colors[HephGuiCol_Border];
    colors[HephGuiCol_DragDropTarget]         = glm::vec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[HephGuiCol_DragDropTargetBg]       = glm::vec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[HephGuiCol_UnsavedMarker]          = glm::vec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[HephGuiCol_NavCursor]              = colors[HephGuiCol_HeaderHovered];
    colors[HephGuiCol_NavWindowingHighlight]  = glm::vec4(0.70f, 0.70f, 0.70f, 0.70f);
    colors[HephGuiCol_NavWindowingDimBg]      = glm::vec4(0.20f, 0.20f, 0.20f, 0.20f);
    colors[HephGuiCol_ModalWindowDimBg]       = glm::vec4(0.20f, 0.20f, 0.20f, 0.35f);
    */
}

void  HephDrawList::addLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness) {
  pathLineTo(p1);
  pathLineTo(p2);
  pathStroke(color, thickness);
}

void  HephDrawList::addRect(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, float thickness) {
  pathLineTo(min + thickness / 2.0f);
  pathLineTo(glm::vec2(max.x - thickness / 2.0f, min.y + thickness / 2.0f));
  pathLineTo(max - thickness / 2.0f);
  pathLineTo(glm::vec2(min.x + thickness / 2.0f, max.y - thickness / 2.0f));
  pathLineTo(min + thickness / 2.0f);
  
  pathStroke(color, thickness);
}

float HephDrawList::addGlyphRect(const HephFont* font, const HephFontFace& face, const glm::vec2& pos, int c, float size, const glm::vec4& color) {
  const HephFontGlyph&    glyph = face.getGlyph(c);
  const HephTextureArea&  area = font->getTextureAtlas().getArea(c + face.glyphOffsetInTex);
  if (size < 1.0f) {
    size = font->getPixelSize();
  }
  float                   scale = size / font->getPixelSize();

  glm::vec2 max(pos.x + (glyph.width + glyph.left) * scale, pos.y + (font->getBaseline() - glyph.top + glyph.height) * scale);
  glm::vec2 min(pos.x + glyph.left * scale, max.y - glyph.height * scale);
  addRectFill(min, max, color, area);

  return (glyph.advance * scale);
}

void  HephDrawList::addText(const std::string& text, float size, const glm::vec4& clipRect, const glm::vec4& color) {
  glm::vec2       pos = glm::vec2((int)clipRect.x, (int)clipRect.y);
  const HephFont* font = _Data->font;
  if (size < 1.0f) {
    size = font->getPixelSize();
  }
  float           lineSize = size;
  glm::vec2       rect = pos;

  for (int i = 0; i < text.size(); i++) {
    if (text[i] == '\n') {
      pos.y += lineSize;
      rect.y = pos.y;
      pos.x = clipRect.x;
    }
    else if (text[i] > 0) {
      if (pos.x >= clipRect.x + clipRect.z)
        continue;
      if (pos.y >= clipRect.y + clipRect.w)
        break;

      pos.x += addGlyphRect(font, font->faceRegular, pos, font->faceRegular.getGlyphIndex(text[i]), size, color);
      if (rect.x < pos.x)
        rect.x = pos.x;
    }
  }
}

glm::vec2 HephDrawList::getTextSize(const std::string& text, float size) {
  float           currentLine = 0.0;
  const HephFont* font = _Data->font;
  if (size < 1.0f) {
    size = font->getPixelSize();
  }
  float           lineSize = size;
  glm::vec2       clipRect = glm::vec2(0.0, lineSize);
  float           scale = size / font->getPixelSize();

  for (int i = 0; i < text.size(); i++) {
    if (text[i] == '\n') {
      clipRect.y += lineSize;
      if (clipRect.x < currentLine)
        clipRect.x = currentLine;
      currentLine = 0;
    }
    else if (text[i] > 0) {
      const HephFontGlyph&    glyph = font->faceRegular.getGlyph(text[i]);
      currentLine += glyph.advance * scale;
    }
  }
  if (clipRect.x < currentLine)
    clipRect.x = currentLine;
  return (clipRect);
}

void  HephDrawList::addRectFill(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, const HephTextureArea& area) {
  idxBufferAdd(vtxBufferCount());
  idxBufferAdd(vtxBufferCount() + 1);
  idxBufferAdd(vtxBufferCount() + 3);
  idxBufferAdd(vtxBufferCount() + 1);
  idxBufferAdd(vtxBufferCount() + 2);
  idxBufferAdd(vtxBufferCount() + 3);

  vtxBufferAdd((HephVertex){.pos = min, .uv = area.min, .color = color});
  vtxBufferAdd((HephVertex){.pos = glm::vec2(max.x, min.y), .uv = {area.max.x, area.min.y}, .color = color});
  vtxBufferAdd((HephVertex){.pos = max, .uv = area.max, .color = color});
  vtxBufferAdd((HephVertex){.pos = glm::vec2(min.x, max.y), .uv = {area.min.x, area.max.y}, .color = color});

  drawCmdBuffer.back().elemCount += 6;
}

void  HephDrawList::addPolyline(const glm::vec2* points, uint32_t size, const glm::vec4& color, float thickness) {
  for (int i = 0; i < size - 1; i++) {
    glm::vec2 nTmp = points[i] - points[i + 1];
    glm::vec2 n1 = glm::normalize(glm::vec2(-nTmp.y, nTmp.x)) * thickness / 2.0f;
    glm::vec2 n2 = glm::normalize(glm::vec2(nTmp.y, -nTmp.x)) * thickness / 2.0f;

    glm::vec2 p1 = points[i] + n1;
    glm::vec2 p2 = points[i + 1] + n1;
    glm::vec2 p3 = points[i + 1] + n2;
    glm::vec2 p4 = points[i] + n2;

    idxBufferAdd(vtxBufferCount());
    idxBufferAdd(vtxBufferCount() + 1);
    idxBufferAdd(vtxBufferCount() + 3);
    idxBufferAdd(vtxBufferCount() + 1);
    idxBufferAdd(vtxBufferCount() + 2);
    idxBufferAdd(vtxBufferCount() + 3);

    vtxBufferAdd((HephVertex){.pos = p1, .uv = _Data->whitePixelArea.min, .color = color});
    vtxBufferAdd((HephVertex){.pos = p2, .uv = glm::vec2(_Data->whitePixelArea.max.x, _Data->whitePixelArea.min.y), .color = color});
    vtxBufferAdd((HephVertex){.pos = p3, .uv = _Data->whitePixelArea.max, .color = color});
    vtxBufferAdd((HephVertex){.pos = p4, .uv = glm::vec2(_Data->whitePixelArea.min.x, _Data->whitePixelArea.max.y), .color = color});

    drawCmdBuffer.back().elemCount += 6;
  }
}

void  HephDrawList::pushClipRect(const glm::ivec2& clipRect_min, const glm::ivec2& clipRect_max, bool intersect_with_current_clipRect) {
  glm::ivec4 cr = glm::ivec4(clipRect_min.x, clipRect_min.y, clipRect_max.x, clipRect_max.y);

  if (intersect_with_current_clipRect) {
    glm::ivec4 current = _ClipRectStack.top();
    if (cr.x < current.x)
      cr.x = current.x;
    if (cr.y < current.y)
      cr.y = current.y;
    if (cr.x < current.z)
      cr.z = current.z;
    if (cr.w > current.w)
      cr.w = current.w;
  }

  cr.z = std::max(cr.x, cr.z);
  cr.w = std::max(cr.y, cr.w);

  _ClipRectStack.push(cr);
}

void  HephDrawList::pushClipRectFullScreen() {
  _ClipRectStack.push(_Data->clipRectFullScreen);
}

void  HephDrawList::popClipRect(bool letEmpty) {
  _ClipRectStack.pop();
  if (!letEmpty && _ClipRectStack.empty())
    pushClipRectFullScreen();
}

}
