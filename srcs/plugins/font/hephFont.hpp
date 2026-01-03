#pragma once

#include "../../core/hephaestus_core.hpp"
#include "../../memory/hephMemoryAllocator.hpp"
#include "../../core/hephResult.hpp"
#include "hephPluginFont.hpp"
#include "../../texture/hephTexture.hpp"
#include <cstdint>
#include <vector>

struct  HephFontCreateInfo {
	const char*           fontFilePathRegular;	
	const char*           fontFilePathBold;	
  int                   faceIndex = -1;
  int                   faceIndexItalic = -1;
  int                   pixelSize;
  HephMemoryAllocator   *mAllocator = nullptr;
  HephCommandPool       *cmdPool = nullptr;
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

struct  HephFontFace {
  void        destroy() {
    if (isLoaded)
      FT_Done_Face(face);
    isLoaded = false;
  }

  HephResult                      load(const char* fontFilePath, int faceIndex, int pixelSize);
  HephResult                      loadGlyphs(uint32_t& width, uint32_t& height, uint32_t& rowCount, int maxWidth);
  HephResult                      loadGlyphInSlot(int index) {
    assert(isLoaded && "Face isn't loaded");
    FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
    FT_Error  error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    return ((HephPluginFont::ftError(error).errorFormat("failed to render glyph {{}} !")));
  }

  inline const uint32_t           getGlyphIndex(uint32_t unicode) const {
    assert(isLoaded && "Face isn't loaded");
    return (FT_Get_Char_Index(face, unicode));
  }
  inline const HephFontGlyph&     getGlyph(uint32_t glyphIndex) const {
    assert(isLoaded && "Face isn't loaded");
    return (glyphs[glyphIndex]);
  }

  uint32_t                    glyphOffsetInTex = 0;
  FT_Face                     face;
  int                         faceIndex = -1;
  std::vector<HephFontGlyph>  glyphs;
  bool                        isLoaded = false;
};

class   HephFont {
  public:
    HephFont() {};
    ~HephFont() {destroy();}

    HephResult                      load(HephFontCreateInfo& createInfo);
    HephResult                      destroy();

    inline const int&               getPixelSize() const {return (m_pixelSize);}
    inline const HephTextureAtlas&  getTextureAtlas() const {return (m_textureAtlas);}
    inline float                    getBaseline() const {return (m_baseline);}

    HephFontFace                faceRegular;
    HephFontFace                faceBold;
    HephFontFace                faceRegularItalic;
    HephFontFace                faceBoldItalic;

  protected:
    HephResult  loadGlyphs(HephFontCreateInfo& createInfo);

    const char*                 m_fontFilePathRegular;
    const char*                 m_fontFilePathBold;
    int                         m_pixelSize;

    bool                        m_hasKerning;
    HephTextureAtlas            m_textureAtlas;
    float                       m_baseline;


};
