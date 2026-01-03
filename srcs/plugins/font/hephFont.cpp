#include "hephFont.hpp"
#include <cstdint>
#include <sys/types.h>
#include <vulkan/vulkan_core.h>
#include "../../texture/hephTexture.hpp"
#include "core/hephResult.hpp"

HephResult  HephFont::load(HephFontCreateInfo& createInfo) {
  HEPH_CHECK_RESULT(HephPluginFont::checkInit());
  HEPH_CHECK_RESULT(HephResult(createInfo.mAllocator != nullptr).errorFormat("mAllocator field of HephFontCreateInfo cannot be NULL"));
  HEPH_CHECK_RESULT(HephResult(createInfo.cmdPool != nullptr).errorFormat("cmdPool field of HephFontCreateInfo cannot be NULL"));

  m_fontFilePathRegular = createInfo.fontFilePathRegular;
  m_fontFilePathBold = createInfo.fontFilePathBold;
  m_pixelSize = createInfo.pixelSize;

  if (std::string(m_fontFilePathRegular) != "") {
    if (createInfo.faceIndex > -1)
      HEPH_CHECK_RESULT(faceRegular.load(m_fontFilePathRegular, createInfo.faceIndex, m_pixelSize));
    if (createInfo.faceIndexItalic > -1)
      HEPH_CHECK_RESULT(faceRegularItalic.load(m_fontFilePathRegular, createInfo.faceIndexItalic, m_pixelSize));
  }
  if (std::string(m_fontFilePathBold) != "") {
    if (createInfo.faceIndex > -1)
      HEPH_CHECK_RESULT(faceBold.load(m_fontFilePathBold, createInfo.faceIndex, m_pixelSize));
    if (createInfo.faceIndexItalic > -1)
      HEPH_CHECK_RESULT(faceBoldItalic.load(m_fontFilePathBold, createInfo.faceIndexItalic, m_pixelSize));
  }

  HEPH_CHECK_RESULT(loadGlyphs(createInfo));
  //m_baseline = ((float)m_face->ascender / (float)m_face->units_per_EM) * (float)m_pixelSize;
  m_baseline = m_pixelSize - (faceRegular.getGlyph(faceRegular.getGlyphIndex('|')).height - faceRegular.getGlyph(faceRegular.getGlyphIndex('|')).top);
  m_hasKerning = FT_HAS_KERNING(faceRegular.face);

  return (HephResult());
}

HephResult  HephFont::destroy() {
  faceRegular.destroy();
  faceRegularItalic.destroy();
  faceBold.destroy();
  faceBoldItalic.destroy();
  return (HephResult());
}

HephResult   HephFontFace::load(const char* fontFilePath, int faceIndex, int pixelSize) {
  FT_Error  error = FT_New_Face(HephPluginFont::getLib(), fontFilePath, faceIndex, &face);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to load font {{}} !"));
  error = FT_Set_Pixel_Sizes(face, 0, pixelSize);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to set pixel size {{}} !"));
  isLoaded = true;
  faceIndex = faceIndex;
  return (HephResult());
}

HephResult  HephFontFace::loadGlyphs(uint32_t& width, uint32_t& height, uint32_t& rowCount, int maxWidth) {
  uint32_t  rowHeight = 0;
  uint32_t  curW = 0;
  rowCount = 0;
  width = 0;
  height = 0;

  if (!isLoaded)
    return (HephResult());

  glyphs.reserve(face->num_glyphs);
  for (int i = 0; i < face->num_glyphs; i++) {
    //FT_UInt   glyphIndex = FT_Get_Char_Index(m_face, i);
    FT_Error  error = FT_Load_Glyph(face, i, FT_LOAD_DEFAULT);
    HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to load glyph {{}} !"));
    glyphs.push_back(HephFontGlyph(face->glyph));

    rowHeight = std::max(rowHeight, face->glyph->bitmap.rows);
    if (rowCount == 0)
      width += face->glyph->bitmap.width;

    curW += face->glyph->bitmap.width;
    if (curW > maxWidth) {
      curW = face->glyph->bitmap.width;
      rowCount++;
      height += rowHeight;
      rowHeight = 0;
    }
  }
  rowCount++;
  if (rowCount > 0)
    width = maxWidth;
  height += rowHeight;
  return (HephResult());
}

HephResult  HephFont::loadGlyphs(HephFontCreateInfo& createInfo) {
  uint32_t  width = 0;
  uint32_t  height = 0;
  uint32_t  rowCount = 0;
  uint32_t  maxWidth = 4096;

  std::vector<HephFontFace*> fontFaces;
  if (faceRegular.isLoaded)
    fontFaces.push_back(&faceRegular);
  if (faceRegularItalic.isLoaded)
    fontFaces.push_back(&faceRegularItalic);
  if (faceBold.isLoaded)
    fontFaces.push_back(&faceBold);
  if (faceBoldItalic.isLoaded)
    fontFaces.push_back(&faceBoldItalic);

  int       glyphCount = 0;
  for (auto* face: fontFaces) {
    uint32_t  widthTmp = 0;
    uint32_t  heightTmp = 0;
    uint32_t  rowCountTmp = 0;
    HEPH_CHECK_RESULT(face->loadGlyphs(widthTmp, heightTmp, rowCountTmp, maxWidth));
    face->glyphOffsetInTex = glyphCount;
    glyphCount += face->face->num_glyphs;
    width = std::max(width, widthTmp);
    height += heightTmp;
    rowCount += rowCountTmp;
  }

  uint32_t  widthTex = std::min(width, maxWidth);
  uint32_t  heightTex = height + 2;
  uint32_t  bufferSize = widthTex * heightTex;
  std::vector<uint8_t>  buffer(bufferSize, 0);
  
  int dst = 0;
  int dstLine = 0;
  int texX = 0;
  int texY = 0;
  uint32_t rowHeight = 0;

  for (auto* face: fontFaces) {
    for (int i = 0; i < face->face->num_glyphs; i++) {
      HEPH_CHECK_RESULT(face->loadGlyphInSlot(i));

      int glyphWidth = face->face->glyph->bitmap.width;
      int glyphHeight = face->face->glyph->bitmap.rows;

      if (texX + glyphWidth > widthTex) {
        dstLine += rowHeight * widthTex;
        dst = dstLine;
        texX = 0;
        texY += rowHeight;
        rowHeight = 0;
      }

      rowHeight = std::max(rowHeight, face->face->glyph->bitmap.rows);
      uint8_t*  src = face->face->glyph->bitmap.buffer;
      for (int y = 0; y < glyphHeight; y++) {
        uint8_t*  srcTmp = src;
        int       dstTmp = dst + y * widthTex;
        for (int x = 0; x < glyphWidth; x++) {
          buffer[dstTmp++] = *srcTmp;
          srcTmp++;
        }
        src += face->face->glyph->bitmap.pitch;
      }
      m_textureAtlas.addArea((HephTextureArea){
          .min = glm::vec2((float)texX / widthTex, (float)texY / heightTex),
          .max = glm::vec2((float)(texX + glyphWidth) / widthTex, (float)(texY + glyphHeight) / heightTex),
          });
      dst += glyphWidth;
      texX += glyphWidth;
    }
  }

  buffer[0 + widthTex * height] = 255;
  buffer[1 + widthTex * height] = 255;
  buffer[0 + widthTex * (height + 1)] = 255;
  buffer[1 + widthTex * (height + 1)] = 255;
  m_textureAtlas.whitePixelArea = (HephTextureArea){
    .min = glm::vec2(0.0, (float)height / heightTex),
    .max = glm::vec2(2.0 / (float)widthTex, 1.0),
  };

  m_textureAtlas.image.format = VK_FORMAT_R8_UINT;

  VkExtent3D extent = {
    .width = widthTex,
    .height = heightTex, 
    .depth = 1,
  };
  VkImageUsageFlagBits  usage = static_cast<VkImageUsageFlagBits>(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
      | VK_IMAGE_USAGE_SAMPLED_BIT
      | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

  VkComponentMapping  components = {
    .a = VK_COMPONENT_SWIZZLE_R,
  };
	HephImageCreateInfo	imageCreateInfo = {
		.extent = extent,
		.usage = usage,
		.layout = VK_IMAGE_LAYOUT_GENERAL,
		.aspect = VK_IMAGE_ASPECT_COLOR_BIT,
		.components = components,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.samples = VK_SAMPLE_COUNT_1_BIT,
	};

  HEPH_CHECK_RESULT(createInfo.mAllocator->createImage(imageCreateInfo, m_textureAtlas.image, *createInfo.cmdPool)
      .errorFormat("Failed to create emulator image {{}} !"));
  VkBufferImageCopy	imgRegion = {
    .bufferOffset = 0,
    .bufferRowLength = 0,
    .bufferImageHeight = 0,
    .imageSubresource = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .mipLevel = 0,
      .baseArrayLayer = 0,
      .layerCount = 1,
    },
    .imageOffset = {.x = 0, .y = 0, .z = 0},
    .imageExtent = m_textureAtlas.image.extent,
  };
  return (createInfo.mAllocator->stagingMakeAndCopyImage(m_textureAtlas.image, imgRegion, buffer.data(), bufferSize, *createInfo.cmdPool));
}
