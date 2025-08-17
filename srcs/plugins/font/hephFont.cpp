#include "hephFont.hpp"

HephResult  HephFont::load(HephFontCreateInfo& createInfo) {
  HEPH_CHECK_RESULT(HephPluginFont::checkInit());
  m_fontFilePath = createInfo.fontFilePath;
  m_faceIndex = createInfo.faceIndex;
  m_pixelSize = createInfo.pixelSize;

  FT_Error  error = FT_New_Face(HephPluginFont::ftLibrary, m_fontFilePath.c_str(), m_faceIndex, &m_face);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to load font {{}} !"));
  error = FT_Set_Pixel_Sizes(m_face, 0, m_pixelSize);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to set pixel size {{}} !"));

  HEPH_CHECK_RESULT(loadGlyphs());
  m_hasKerning = FT_HAS_KERNING(m_face);

  return (HephResult());
}

HephResult  HephFont::loadGlyphs() {
  uint32_t  width = 0;
  uint32_t  height = 0;
  m_glyphs.reserve(m_face->num_glyphs);
  for (int i = 0; i < m_face->num_glyphs; i++) {
    FT_UInt   glyphIndex = FT_Get_Char_Index(m_face, i);
    FT_Error  error = FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_DEFAULT);
    HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to load glyph {{}} !"));
    m_glyphs.push_back(HephFontGlyph(m_face->glyph));
    height = std::max(height, m_face->glyph->bitmap.rows);
    width += m_face->glyph->bitmap.width;
  }

  int bufferSize = width*height*4;
  std::vector<uint8_t>  buffer(bufferSize, 0);
  
  int dst = 0;

  for (int i = 0; i < m_face->num_glyphs; i++) {
    FT_UInt   glyphIndex = FT_Get_Char_Index(m_face, i);
    FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_DEFAULT);
    FT_Error  error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
    HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to render glyph {{}} !"));

    int glyphWidth = m_face->glyph->bitmap.width;
    int glyphHeight = m_face->glyph->bitmap.rows;

    int       dstTmp = dst;
    uint8_t*  src = m_face->glyph->bitmap.buffer;
    for (int y = 0; y < glyphHeight; y++) {
      uint8_t*  srcTmp = src;
      for (int x = 0; x < glyphWidth; x++) {
        buffer[dstTmp++] = 0xFF;
        buffer[dstTmp++] = 0xFF;
        buffer[dstTmp++] = 0xFF;
        buffer[dstTmp++] = *src;
        srcTmp++;
      }
      src += m_face->glyph->bitmap.pitch;
    }
    dst += glyphWidth * height * 4;
  }
  return (HephResult());
}

HephResult  HephFont::destroy() {
  FT_Done_Face(m_face);
  return (HephResult());
}
