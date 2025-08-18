#pragma once

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "../../core/hephResult.hpp"

#include <memory>

namespace HephPluginFont {
HephResult  init();
HephResult  checkInit();
HephResult  ftError(FT_Error);
FT_Library  getLib();
}
