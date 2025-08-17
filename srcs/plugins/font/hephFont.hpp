#pragma once

#include "hephPluginFont.hpp"
#include <vector>

struct  HephFontCreateInfo {
	const char*   fontFilePath;	
  int           faceIndex = -1;
  int           pixelSize;
};

struct  HephFontGlyph {
  HephFontGlyph(FT_GlyphSlot glyph) {
    top = glyph->bitmap_top;
    left = glyph->bitmap_left;
    width = static_cast<int>(glyph->metrics.width >> 6);
    height = static_cast<int>(glyph->metrics.height >> 6);
    advance = static_cast<int>(glyph->advance.x >> 6);
  }
  int top;
  int left;
  int width;
  int height;
  int advance;
};

class   HephFont {
  public:
    HephFont() {};
    ~HephFont() {destroy();}

    HephResult  load(HephFontCreateInfo& createInfo);
    HephResult  destroy();

  protected:
    HephResult  loadGlyphs();

    std::string                 m_fontFilePath;
    int                         m_faceIndex;
    int                         m_pixelSize;
    FT_Face                     m_face;
    std::vector<HephFontGlyph>  m_glyphs;
    bool                        m_hasKerning;
};
