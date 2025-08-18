#include "hephFont.hpp"
#include "../../command/hephCommandPool.hpp"
#include <cstdint>
#include <sys/types.h>
#include <vulkan/vulkan_core.h>

HephResult  HephFont::load(HephDevice& device, HephImageWrapper& texture, HephFontCreateInfo& createInfo) {
  HEPH_CHECK_RESULT(HephPluginFont::checkInit());
  m_fontFilePath = createInfo.fontFilePath;
  m_faceIndex = createInfo.faceIndex;
  m_pixelSize = createInfo.pixelSize;

  FT_Error  error = FT_New_Face(HephPluginFont::getLib(), m_fontFilePath, m_faceIndex, &m_face);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to load font {{}} !"));
  error = FT_Set_Pixel_Sizes(m_face, 0, m_pixelSize);
  HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to set pixel size {{}} !"));

  HEPH_CHECK_RESULT(loadGlyphs(device, texture));
  m_hasKerning = FT_HAS_KERNING(m_face);

  return (HephResult());
}

HephResult  HephFont::destroy() {
  FT_Done_Face(m_face);
  return (HephResult());
}

HephResult  HephFont::loadGlyphs(HephDevice& device, HephImageWrapper& texture) {
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
  w = 0;
  rowHeight = 0;

  for (int i = 0; i < m_face->num_glyphs; i++) {
    FT_UInt   glyphIndex = FT_Get_Char_Index(m_face, i);
    FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_DEFAULT);
    FT_Error  error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
    HEPH_CHECK_RESULT(HephPluginFont::ftError(error).errorFormat("failed to render glyph {{}} !"));

    int glyphWidth = m_face->glyph->bitmap.width;
    int glyphHeight = m_face->glyph->bitmap.rows;

    if (w + glyphWidth > widthTex) {
      dstLine += rowHeight * widthTex;
      dst = dstLine;
      w = 0;
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
    dst += glyphWidth;
    w += glyphWidth;
  }

  texture.format = VK_FORMAT_R8_UINT;

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

  HephMemoryAllocator   memoryAllocator;
  HephCommandPool       cmdPool;
  HephCommandPoolCreateInfo cmdPoolCreateInfo = {
	  .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
	  .queueFamilyIndex = device.queues[0].familyIndex,
  };
  HEPH_CHECK_RESULT(cmdPool.create(device, cmdPoolCreateInfo));
  HEPH_CHECK_RESULT(memoryAllocator.create(device));
  
  HEPH_CHECK_RESULT(memoryAllocator.createImage(imageCreateInfo, texture, cmdPool).errorFormat("Failed to create emulator image {{}} !"));
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
    .imageExtent = texture.extent,
  };
  return (memoryAllocator.stagingMakeAndCopyImage(texture, imgRegion, buffer.data(), bufferSize, cmdPool));
}
