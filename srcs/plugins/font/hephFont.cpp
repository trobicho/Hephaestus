#include "hephFont.hpp"
#include <cstdint>
#include <sys/types.h>
#include <vulkan/vulkan_core.h>
#include "../../texture/hephTexture.hpp"

HephResult  HephFont::load(HephFontCreateInfo& createInfo) {
  HEPH_CHECK_RESULT(HephPluginFont::checkInit());
  HEPH_CHECK_RESULT(HephResult(createInfo.mAllocator != nullptr).errorFormat("mAllocator field of HephFontCreateInfo cannot be NULL"));
  HEPH_CHECK_RESULT(HephResult(createInfo.cmdPool != nullptr).errorFormat("cmdPool field of HephFontCreateInfo cannot be NULL"));
  HEPH_CHECK_RESULT(HephResult(createInfo.textureAtlas != nullptr).errorFormat("textureAtlas field of HephFontCreateInfo cannot be NULL"));

  m_fontFilePath = createInfo.fontFilePath;
  m_faceIndex = createInfo.faceIndex;
  m_pixelSize = createInfo.pixelSize;

  FT_Error  error = FT_New_Face(HephPluginFont::getLib(), m_fontFilePath, m_faceIndex, &m_face);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to load font {{}} !"));
  error = FT_Set_Pixel_Sizes(m_face, 0, m_pixelSize);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to set pixel size {{}} !"));

  HEPH_CHECK_RESULT(loadGlyphs(createInfo));
  m_hasKerning = FT_HAS_KERNING(m_face);

  return (HephResult());
}

HephResult  HephFont::destroy() {
  FT_Done_Face(m_face);
  return (HephResult());
}

HephResult  HephFont::loadGlyphs(HephFontCreateInfo& createInfo) {
  uint32_t  width = 0;
  uint32_t  height = 0;
  uint32_t  maxWidth = 4096;
  uint32_t  w = 0;
  uint32_t  rows = 1;
  uint32_t  rowHeight = 0;
  m_glyphs.reserve(m_face->num_glyphs);
  for (int i = 0; i < m_face->num_glyphs; i++) {
    FT_UInt   glyphIndex = FT_Get_Char_Index(m_face, i);
    FT_Error  error = FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_DEFAULT);
    HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to load glyph {{}} !"));
    m_glyphs.push_back(HephFontGlyph(m_face->glyph));
    rowHeight = std::max(rowHeight, m_face->glyph->bitmap.rows);
    width += m_face->glyph->bitmap.width;
    w +=  m_face->glyph->bitmap.width;
    if (w > maxWidth) {
      w = m_face->glyph->bitmap.width;
      rows++;
      height += rowHeight;
      rowHeight = 0;
    }
  }
  height += rowHeight;

  uint32_t  widthTex = std::min(width, maxWidth);
  uint32_t  heightTex = height;
  uint32_t  bufferSize = widthTex * heightTex;
  std::vector<uint8_t>  buffer(bufferSize, 0);
  
  int dst = 0;
  int dstLine = 0;
  int texX = 0;
  int texY = 0;
  rowHeight = 0;

  for (int i = 0; i < m_face->num_glyphs; i++) {
    FT_UInt   glyphIndex = FT_Get_Char_Index(m_face, i);
    FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_DEFAULT);
    FT_Error  error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
    HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to render glyph {{}} !"));

    int glyphWidth = m_face->glyph->bitmap.width;
    int glyphHeight = m_face->glyph->bitmap.rows;

    if (texX + glyphWidth > widthTex) {
      dstLine += rowHeight * widthTex;
      dst = dstLine;
      texX = 0;
      texY += rowHeight;
      rowHeight = 0;
    }

    rowHeight = std::max(rowHeight, m_face->glyph->bitmap.rows);
    uint8_t*  src = m_face->glyph->bitmap.buffer;
    for (int y = 0; y < glyphHeight; y++) {
      uint8_t*  srcTmp = src;
      int       dstTmp = dst + y * widthTex;
      for (int x = 0; x < glyphWidth; x++) {
        buffer[dstTmp++] = *srcTmp;
        srcTmp++;
      }
      src += m_face->glyph->bitmap.pitch;
    }
    createInfo.textureAtlas->addArea((HephTextureArea){
      .min = glm::vec2((float)texX / widthTex, (float)texY / height),
      .max = glm::vec2((float)(texX + glyphWidth) / widthTex, (float)(texY + glyphHeight) / height),
    });
    dst += glyphWidth;
    texX += glyphWidth;
  }

  createInfo.textureAtlas->image.format = VK_FORMAT_R8_UINT;

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

  HEPH_CHECK_RESULT(createInfo.mAllocator->createImage(imageCreateInfo, createInfo.textureAtlas->image, *createInfo.cmdPool)
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
    .imageExtent = createInfo.textureAtlas->image.extent,
  };
  return (createInfo.mAllocator->stagingMakeAndCopyImage(createInfo.textureAtlas->image, imgRegion, buffer.data(), bufferSize, *createInfo.cmdPool));
}
