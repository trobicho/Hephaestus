#include "hephGuiInternal.hpp"
#include "hephGui.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>

namespace HephGui {

/*
namespace HephStb {

#include "libs/stb_textedit.h"

static int     STB_TEXTEDIT_STRINGLEN(const HephGuiInputTextState* obj) { return obj->textLen; }
static char    STB_TEXTEDIT_GETCHAR(const HephGuiInputTextState* obj, int idx) {
  HEPHGUI_ASSERT(idx >= 0 && idx <= obj->textLen);
  return obj->textSrc[idx];
}
static float   STB_TEXTEDIT_GETWIDTH(HephGuiInputTextState* obj, int line_start_idx, int char_idx) {
  unsigned int c;
  ImTextCharFromUtf8(&c, obj->textSrc + line_start_idx + char_idx, obj->textSrc + obj->textLen);
  if ((ImWchar)c == '\n')
    return IMSTB_TEXTEDIT_GETWIDTH_NEWLINE;
  HephGuiContext& g = *obj->ctx;
  return g.FontBaked->GetCharAdvance((ImWchar)c) * g.FontBakedScale;
}
static char    STB_TEXTEDIT_NEWLINE = '\n';
static void    STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r, HephGuiInputTextState* obj, int line_start_idx) {
    const char* text = obj->textSrc;
    const char* text_remaining = NULL;
    const ImVec2 size = InputTextCalcTextSize(obj->ctx, text + line_start_idx, text + obj->textLen, text + obj->textLen, &text_remaining, NULL, ImDrawTextFlags_StopOnNewLine | ImDrawTextFlags_WrapKeepBlanks);
    r->x0 = 0.0f;
    r->x1 = size.x;
    r->baseline_y_delta = size.y;
    r->ymin = 0.0f;
    r->ymax = size.y;
    r->num_chars = (int)(text_remaining - (text + line_start_idx));
}

#define IMSTB_TEXTEDIT_GETNEXTCHARINDEX  IMSTB_TEXTEDIT_GETNEXTCHARINDEX_IMPL
#define IMSTB_TEXTEDIT_GETPREVCHARINDEX  IMSTB_TEXTEDIT_GETPREVCHARINDEX_IMPL

static int IMSTB_TEXTEDIT_GETNEXTCHARINDEX_IMPL(HephGuiInputTextState* obj, int idx) {
    if (idx >= obj->textLen)
        return obj->textLen + 1;
    unsigned int c;
    return idx + ImTextCharFromUtf8(&c, obj->textSrc + idx, obj->textSrc + obj->textLen);
}

static int IMSTB_TEXTEDIT_GETPREVCHARINDEX_IMPL(HephGuiInputTextState* obj, int idx) {
    if (idx <= 0)
        return -1;
    const char* p = ImTextFindPreviousUtf8Codepoint(obj->textSrc, obj->textSrc + idx);
    return (int)(p - obj->textSrc);
}

static bool ImCharIsSeparatorW(unsigned int c) {
    static const unsigned int separator_list[] =
    {
        ',', 0x3001, '.', 0x3002, ';', 0xFF1B, '(', 0xFF08, ')', 0xFF09, '{', 0xFF5B, '}', 0xFF5D,
        '[', 0x300C, ']', 0x300D, '|', 0xFF5C, '!', 0xFF01, '\\', 0xFFE5, '/', 0x30FB, 0xFF0F,
        '\n', '\r',
    };
    for (unsigned int separator : separator_list)
        if (c == separator)
            return true;
    return false;
}

static int is_word_boundary_from_right(HephGuiInputTextState* obj, int idx) {
    // When HephGuiInputTextFlags_Password is set, we don't want actions such as Ctrl+Arrow to leak the fact that underlying data are blanks or separators.
    if ((obj->Flags & HephGuiInputTextFlags_Password) || idx <= 0)
        return 0;

    const char* curr_p = obj->textSrc + idx;
    const char* prev_p = ImTextFindPreviousUtf8Codepoint(obj->textSrc, curr_p);
    unsigned int curr_c; ImTextCharFromUtf8(&curr_c, curr_p, obj->textSrc + obj->textLen);
    unsigned int prev_c; ImTextCharFromUtf8(&prev_c, prev_p, obj->textSrc + obj->textLen);

    bool prev_white = ImCharIsBlankW(prev_c);
    bool prev_separ = ImCharIsSeparatorW(prev_c);
    bool curr_white = ImCharIsBlankW(curr_c);
    bool curr_separ = ImCharIsSeparatorW(curr_c);
    return ((prev_white || prev_separ) && !(curr_separ || curr_white)) || (curr_separ && !prev_separ);
}
static int is_word_boundary_from_left(HephGuiInputTextState* obj, int idx) {
    if ((obj->Flags & HephGuiInputTextFlags_Password) || idx <= 0)
        return 0;

    const char* curr_p = obj->textSrc + idx;
    const char* prev_p = ImTextFindPreviousUtf8Codepoint(obj->textSrc, curr_p);
    unsigned int prev_c; ImTextCharFromUtf8(&prev_c, curr_p, obj->textSrc + obj->textLen);
    unsigned int curr_c; ImTextCharFromUtf8(&curr_c, prev_p, obj->textSrc + obj->textLen);

    bool prev_white = ImCharIsBlankW(prev_c);
    bool prev_separ = ImCharIsSeparatorW(prev_c);
    bool curr_white = ImCharIsBlankW(curr_c);
    bool curr_separ = ImCharIsSeparatorW(curr_c);
    return ((prev_white) && !(curr_separ || curr_white)) || (curr_separ && !prev_separ);
}
static int  STB_TEXTEDIT_MOVEWORDLEFT_IMPL(HephGuiInputTextState* obj, int idx) {
    idx = IMSTB_TEXTEDIT_GETPREVCHARINDEX(obj, idx);
    while (idx >= 0 && !is_word_boundary_from_right(obj, idx))
        idx = IMSTB_TEXTEDIT_GETPREVCHARINDEX(obj, idx);
    return idx < 0 ? 0 : idx;
}
static int  STB_TEXTEDIT_MOVEWORDRIGHT_MAC(HephGuiInputTextState* obj, int idx) {
    int len = obj->textLen;
    idx = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
    while (idx < len && !is_word_boundary_from_left(obj, idx))
        idx = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
    return idx > len ? len : idx;
}
static int  STB_TEXTEDIT_MOVEWORDRIGHT_WIN(HephGuiInputTextState* obj, int idx) {
    idx = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
    int len = obj->textLen;
    while (idx < len && !is_word_boundary_from_right(obj, idx))
        idx = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
    return idx > len ? len : idx;
}
static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(HephGuiInputTextState* obj, int idx) {
  HephGuiContext& g = *obj->ctx;
  if (g.IO.ConfigMacOSXBehaviors)
    return STB_TEXTEDIT_MOVEWORDRIGHT_MAC(obj, idx);
  else
    return STB_TEXTEDIT_MOVEWORDRIGHT_WIN(obj, idx);
}
#define STB_TEXTEDIT_MOVEWORDLEFT       STB_TEXTEDIT_MOVEWORDLEFT_IMPL  // They need to be #define for stb_textedit.h
#define STB_TEXTEDIT_MOVEWORDRIGHT      STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

// Reimplementation of stb_textedit_move_line_start()/stb_textedit_move_line_end() which supports word-wrapping.
static int STB_TEXTEDIT_MOVELINESTART_IMPL(HephGuiInputTextState* obj, ImStb::STB_TexteditState* state, int cursor) {
    if (state->single_line)
        return 0;

    if (obj->WrapWidth > 0.0f) {
        HephGuiContext& g = *obj->ctx;
        const char* p_cursor = obj->textSrc + cursor;
        const char* p_bol = ImStrbol(p_cursor, obj->textSrc);
        const char* p = p_bol;
        const char* text_end = obj->textSrc + obj->textLen; // End of line would be enough
        while (p >= p_bol) {
            const char* p_eol = ImFontCalcWordWrapPositionEx(g.Font, g.FontSize, p, text_end, obj->WrapWidth, ImDrawTextFlags_WrapKeepBlanks);
            if (p == p_cursor) // If we are already on a visible beginning-of-line, return real beginning-of-line (would be same as regular handler below)
                return (int)(p_bol - obj->textSrc);
            if (p_eol == p_cursor && obj->text[cursor] != '\n' && obj->LastMoveDirectionLR == HephGuiDir_Left)
                return (int)(p_bol - obj->textSrc);
            if (p_eol >= p_cursor)
                return (int)(p - obj->textSrc);
            p = (*p_eol == '\n') ? p_eol + 1 : p_eol;
        }
    }

    // Regular handler, same as stb_textedit_move_line_start()
    while (cursor > 0) {
        int prev_cursor = IMSTB_TEXTEDIT_GETPREVCHARINDEX(obj, cursor);
        if (STB_TEXTEDIT_GETCHAR(obj, prev_cursor) == STB_TEXTEDIT_NEWLINE)
            break;
        cursor = prev_cursor;
    }
    return cursor;
}

static int STB_TEXTEDIT_MOVELINEEND_IMPL(HephGuiInputTextState* obj, ImStb::STB_TexteditState* state, int cursor) {
    int n = STB_TEXTEDIT_STRINGLEN(obj);
    if (state->single_line)
        return n;

    if (obj->WrapWidth > 0.0f) {
        HephGuiContext& g = *obj->ctx;
        const char* p_cursor = obj->textSrc + cursor;
        const char* p = ImStrbol(p_cursor, obj->textSrc);
        const char* text_end = obj->textSrc + obj->textLen; // End of line would be enough
        while (p < text_end) {
            const char* p_eol = ImFontCalcWordWrapPositionEx(g.Font, g.FontSize, p, text_end, obj->WrapWidth, ImDrawTextFlags_WrapKeepBlanks);
            cursor = (int)(p_eol - obj->textSrc);
            if (p_eol == p_cursor && obj->LastMoveDirectionLR != HephGuiDir_Left) // If we are already on a visible end-of-line, switch to regular handle
                break;
            if (p_eol > p_cursor)
                return cursor;
            p = (*p_eol == '\n') ? p_eol + 1 : p_eol;
        }
    }
    // Regular handler, same as stb_textedit_move_line_end()
    while (cursor < n && STB_TEXTEDIT_GETCHAR(obj, cursor) != STB_TEXTEDIT_NEWLINE)
        cursor = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, cursor);
    return cursor;
}

#define STB_TEXTEDIT_MOVELINESTART      STB_TEXTEDIT_MOVELINESTART_IMPL
#define STB_TEXTEDIT_MOVELINEEND        STB_TEXTEDIT_MOVELINEEND_IMPL

static void STB_TEXTEDIT_DELETECHARS(HephGuiInputTextState* obj, int pos, int n) {
    // Offset remaining text (+ copy zero terminator)
    HEPHGUI_ASSERT(obj->textSrc == obj->text.Data);
    char* dst = obj->text.Data + pos;
    char* src = obj->text.Data + pos + n;
    memmove(dst, src, obj->textLen - n - pos + 1);
    obj->Edited = true;
    obj->textLen -= n;
}

static int STB_TEXTEDIT_INSERTCHARS(HephGuiInputTextState* obj, int pos, const char* new_text, int new_text_len) {
    const bool is_resizable = (obj->Flags & HephGuiInputTextFlags_CallbackResize) != 0;
    const int text_len = obj->textLen;
    HEPHGUI_ASSERT(pos <= text_len);

    // We support partial insertion (with a mod in stb_textedit.h)
    const int avail = obj->BufCapacity - 1 - obj->textLen;
    if (!is_resizable && new_text_len > avail)
        new_text_len = (int)(ImTextFindValidUtf8CodepointEnd(new_text, new_text + new_text_len, new_text + avail) - new_text); // Truncate to closest UTF-8 codepoint. Alternative: return 0 to cancel insertion.
    if (new_text_len == 0)
        return 0;

    // Grow internal buffer if needed
    HEPHGUI_ASSERT(obj->textSrc == obj->text.Data);
    if (text_len + new_text_len + 1 > obj->text.Size && is_resizable) {
        obj->text.resize(text_len + ImClamp(new_text_len, 32, ImMax(256, new_text_len)) + 1);
        obj->textSrc = obj->text.Data;
    }

    char* text = obj->text.Data;
    if (pos != text_len)
        memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos));
    memcpy(text + pos, new_text, (size_t)new_text_len);

    obj->Edited = true;
    obj->textLen += new_text_len;
    obj->text[obj->textLen] = '\0';

    return new_text_len;
}

// We don't use an enum so we can build even with conflicting symbols (if another user of stb_textedit.h leak their STB_TEXTEDIT_K_* symbols)
#define STB_TEXTEDIT_K_LEFT         0x200000 // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT        0x200001 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP           0x200002 // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN         0x200003 // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART    0x200004 // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND      0x200005 // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART    0x200006 // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND      0x200007 // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE       0x200008 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE    0x200009 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO         0x20000A // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO         0x20000B // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT     0x20000C // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT    0x20000D // keyboard input to move cursor right one word
#define STB_TEXTEDIT_K_PGUP         0x20000E // keyboard input to move cursor up a page
#define STB_TEXTEDIT_K_PGDOWN       0x20000F // keyboard input to move cursor down a page
#define STB_TEXTEDIT_K_SHIFT        0x400000

#define STB_TEXTEDIT_IMPLEMENTATION
#define STB_TEXTEDIT_memmove memmove
#include "libs/stb_textedit.h"

// stb_textedit internally allows for a single undo record to do addition and deletion, but somehow, calling
// the stb_textedit_paste() function creates two separate records, so we perform it manually. (FIXME: Report to nothings/stb?)
static void stb_textedit_replace(HephGuiInputTextState* str, STB_TexteditState* state, const IMSTB_TEXTEDIT_CHARTYPE* text, int text_len) {
    stb_text_makeundo_replace(str, state, 0, str->textLen, text_len);
    ImStb::STB_TEXTEDIT_DELETECHARS(str, 0, str->textLen);
    state->cursor = state->select_start = state->select_end = 0;
    if (text_len <= 0)
        return;
    int text_len_inserted = ImStb::STB_TEXTEDIT_INSERTCHARS(str, 0, text, text_len);
    if (text_len_inserted > 0) {
        state->cursor = state->select_start = state->select_end = text_len;
        state->has_preferred_x = 0;
        return;
    }
    HEPHGUI_ASSERT(0); // Failed to insert character, normally shouldn't happen because of how we currently use stb_textedit_replace()
}

}
*/

bool  ButtonBehavior(const glm::ivec4& rect, HephGuiId id, bool* hovered, bool* held) {
  HephGuiContext& ctx = getContext();

  *hovered = (ctx.hoveredId == id);
  *held = (ctx.activeId == id) & ctx.activeIdIsHeld;

  return (id == ctx.activeId);
}

bool  Button(const std::string& text) {
  HephWindow*   window = GetCurrentWindowPtr();
  HephDrawList* drawList = window->drawList;
  glm::ivec2    textSize = drawList->getTextSize(text, -1.0f);
  HephGuiStyle& style = getContext().style;
  HephGuiId     id = HephHashStr(text.c_str());

  glm::ivec2    clipRectMin = window->frameData.cursorPos;
  glm::ivec2    clipRectMax = clipRectMin + textSize + 10;
  glm::ivec4    clipRect = glm::ivec4(clipRectMin, clipRectMax);
  glm::ivec4    clipRectText = glm::ivec4(clipRectMin + 5, clipRectMax - 5);

  window->itemAdd(clipRect, id);

  bool  hovered = false;
  bool  held = false;
  bool  pressed = ButtonBehavior(clipRect, id, &hovered, &held);

  uint32_t  col = HephGuiCol_Button;
  if (hovered)
    col = HephGuiCol_ButtonHovered;
  if (pressed)
    col = HephGuiCol_ButtonActive;

  window->drawList->addRectFill(clipRectMin, clipRectMax, style.colors[col]);
  window->drawList->addText(text, -1, clipRectText, style.colors[HephGuiCol_Text]);

  return (pressed);
}

bool  MenuItem(const std::string& text) {
  HephWindow*   window = GetCurrentWindowPtr();
  HephDrawList* drawList = window->drawList;
  glm::ivec2    textSize = drawList->getTextSize(text, -1.0f);
  HephGuiStyle& style = getContext().style;
  HephGuiId     id = HephHashStr(text.c_str());

  glm::ivec2    clipRectMin = window->frameData.cursorPos;
  glm::ivec2    clipRectMax = clipRectMin + textSize;
  glm::ivec4    clipRect = glm::ivec4(clipRectMin, clipRectMax);

  window->itemAdd(clipRect, id);

  bool  hovered = false;
  bool  held = false;
  bool  pressed = ButtonBehavior(clipRect, id, &hovered, &held);

  uint32_t  col = HephGuiCol_TextDisabled;
  if (pressed)
    col = HephGuiCol_Text;

  //window->drawList->addRectFill(clipRectMin, clipRectMax, style.colors[col]);
  window->drawList->addText(text, -1, clipRect, style.colors[col]);

  return (false);
}

bool  InputText(const std::string& label, const glm::ivec2& size, char* buf, int bufSize) {
  HephGuiContext& g = getContext();
  HephWindow*     window = GetCurrentWindowPtr();
  HephDrawList*   drawList = window->drawList;
  HephGuiStyle&   style = g.style;
  HephGuiId       id = HephHashStr(label.c_str());

  glm::ivec2    clipRectMin = window->frameData.cursorPos;
  glm::ivec2    clipRectMax = clipRectMin + size + 2;
  glm::ivec4    clipRect = glm::ivec4(clipRectMin + 1, clipRectMax - 1);

  window->itemAdd(clipRect, id);

  bool  hovered = false;
  bool  held = false;
  bool  pressed = ButtonBehavior(clipRect, id, &hovered, &held);

  uint32_t  col = HephGuiCol_TextDisabled;
  if (pressed)
    col = HephGuiCol_Text;

  uint32_t  colBg = HephGuiCol_FrameBg;
  if (hovered)
    colBg = HephGuiCol_FrameBgActive;
  if (pressed)
    colBg = HephGuiCol_FrameBgHovered;

  window->drawList->addRectFill(clipRectMin, clipRectMax, style.colors[colBg]);

  HephGuiInputTextState*  state = g.getInputTextState(id);
  if (state == nullptr && id == g.activeId) {
    state = &g.inputTextState;
    int bufLen = strlen(buf);
    state->textToRevertTo.resize(bufLen + 1);
    memcpy(state->textToRevertTo.data(), buf, bufLen + 1);

    state->id = id;
    state->textLen = bufLen;
    state->text.resize(bufSize + 1);
    memcpy(state->text.data(), buf, bufLen + 1);
  }
  if (state) {
    window->drawList->addText(state->text.data(), state->textLen, -1, clipRect, style.colors[col]);
  }
  else {
    window->drawList->addText(buf, -1, clipRect, style.colors[col]);
  }

  return (false);
}


bool  SliderFloat(const std::string& label, const glm::ivec2& size, float& value, const float min, const float max, bool vertical) {
  HephGuiContext& g = getContext();
  HephWindow*     window = GetCurrentWindowPtr();
  HephDrawList*   drawList = window->drawList;
  HephGuiStyle&   style = getContext().style;
  HephGuiId       id = HephHashStr(label.c_str());

  glm::ivec2      clipRectMin = window->frameData.cursorPos;
  glm::ivec2      clipRectMax = clipRectMin + size;
  glm::ivec4      clipRect = glm::ivec4(clipRectMin, clipRectMax);

  window->itemAdd(clipRect, id);

  bool  hovered = false;
  bool  held = false;
  bool  pressed = ButtonBehavior(clipRect, id, &hovered, &held);

  uint32_t  col = HephGuiCol_ScrollbarGrab;
  uint32_t  colBg = HephGuiCol_FrameBg;
  if (hovered) {
    col = HephGuiCol_ScrollbarGrabHovered;
    colBg = HephGuiCol_FrameBgHovered;
  }
  if (held) {
    col = HephGuiCol_ScrollbarGrabActive;
    colBg = HephGuiCol_FrameBgActive;
  }

  if (!vertical) {
    glm::vec2 minPos = glm::vec2(clipRectMin) + glm::vec2(5.0f, (clipRectMax.y - clipRectMin.y) / 2.0f);
    glm::vec2 maxPos = minPos + glm::vec2(clipRectMax.x - clipRectMin.x - 10.f, 0.0f);
    window->drawList->pathLineTo(minPos);
    window->drawList->pathLineTo(maxPos);
    window->drawList->pathStroke(style.colors[colBg], 2.0f);

    if (held) {
      float curV = (g.cursor.pos.x - minPos.x) / (maxPos.x - minPos.x);
      curV = std::clamp(curV, 0.0f, 1.0f);
      value = ((float)(max - min) * curV) + min;
    }
    float     ratio = (float)(value - min) / (float)(max - min);
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    glm::vec2 pos = minPos + (maxPos - minPos) * ratio;
    window->drawList->addRectFill(glm::vec2(pos.x - 5.0f, clipRectMin.y), glm::vec2(pos.x + 5.0f, clipRectMax.y), style.colors[col]);
  }
  else {
    glm::vec2 minPos = glm::vec2(clipRectMin) + glm::vec2((clipRectMax.x - clipRectMin.x) / 2.0f, 5.0f);
    glm::vec2 maxPos = minPos + glm::vec2(0.0f, clipRectMin.y - clipRectMax.y - 10.f);
    window->drawList->pathLineTo(minPos);
    window->drawList->pathLineTo(maxPos);
    window->drawList->pathStroke(style.colors[colBg], 2.0f);

    if (held) {
      float curV = (g.cursor.pos.y - minPos.y) / (maxPos.y - minPos.y);
      curV = std::clamp(curV, 0.0f, 1.0f);
      value = ((float)(max - min) * curV) + min;
    }
    float     ratio = (float)(value - min) / (float)(max - min);
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    glm::vec2 pos = minPos + (maxPos - minPos) * ratio;
    window->drawList->addRectFill(glm::vec2(clipRectMin.x, pos.y - 5.0f), glm::vec2(clipRectMax.x, pos.y + 5.0f), style.colors[col]);
  }

  return (held);
}

bool  SliderInt(const std::string& label, const glm::ivec2& size, int& value, const int min, const int max, bool vertical) {
  HephGuiContext& g = getContext();
  HephWindow*     window = GetCurrentWindowPtr();
  HephDrawList*   drawList = window->drawList;
  HephGuiStyle&   style = getContext().style;
  HephGuiId       id = HephHashStr(label.c_str());

  glm::ivec2      clipRectMin = window->frameData.cursorPos;
  glm::ivec2      clipRectMax = clipRectMin + size;
  glm::ivec4      clipRect = glm::ivec4(clipRectMin, clipRectMax);

  window->itemAdd(clipRect, id);

  bool  hovered = false;
  bool  held = false;
  bool  pressed = ButtonBehavior(clipRect, id, &hovered, &held);

  uint32_t  col = HephGuiCol_ScrollbarGrab;
  uint32_t  colBg = HephGuiCol_FrameBg;
  if (hovered) {
    col = HephGuiCol_ScrollbarGrabHovered;
    colBg = HephGuiCol_FrameBgHovered;
  }
  if (held) {
    col = HephGuiCol_ScrollbarGrabActive;
    colBg = HephGuiCol_FrameBgActive;
  }

  if (!vertical) {
    glm::vec2 minPos = glm::vec2(clipRectMin) + glm::vec2(5.0f, (clipRectMax.y - clipRectMin.y) / 2.0f);
    glm::vec2 maxPos = minPos + glm::vec2(clipRectMax.x - clipRectMin.x - 10.f, 0.0f);
    window->drawList->pathLineTo(minPos);
    window->drawList->pathLineTo(maxPos);
    window->drawList->pathStroke(style.colors[colBg], 2.0f);

    if (held) {
      float curV = (g.cursor.pos.x - minPos.x) / (maxPos.x - minPos.x);
      curV = std::clamp(curV, 0.0f, 1.0f);
      value = std::round(((float)(max - min) * curV) + min);
    }
    float     ratio = (float)(value - min) / (float)(max - min);
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    glm::vec2 pos = minPos + (maxPos - minPos) * ratio;
    window->drawList->addRectFill(glm::vec2(pos.x - 5.0f, clipRectMin.y), glm::vec2(pos.x + 5.0f, clipRectMax.y), style.colors[col]);
  }
  else {
    glm::vec2 minPos = glm::vec2(clipRectMin) + glm::vec2((clipRectMax.x - clipRectMin.x) / 2.0f, 5.0f);
    glm::vec2 maxPos = minPos + glm::vec2(0.0f, clipRectMin.y - clipRectMax.y - 10.f);
    window->drawList->pathLineTo(minPos);
    window->drawList->pathLineTo(maxPos);
    window->drawList->pathStroke(style.colors[colBg], 2.0f);

    if (held) {
      float curV = (g.cursor.pos.y - minPos.y) / (maxPos.y - minPos.y);
      curV = std::clamp(curV, 0.0f, 1.0f);
      value = std::round(((float)(max - min) * curV) + min);
    }
    float     ratio = (float)(value - min) / (float)(max - min);
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    glm::vec2 pos = minPos + (maxPos - minPos) * ratio;
    window->drawList->addRectFill(glm::vec2(clipRectMin.x, pos.y - 5.0f), glm::vec2(clipRectMax.x, pos.y + 5.0f), style.colors[col]);
  }

  return (held);
}

}
