#pragma once

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "../../core/hephResult.hpp"

#include <memory>

#include "hephFont.hpp"

namespace HephPluginFont {

HephResult  init();
HephResult  checkInit();
HephResult  ftError(FT_Error);

static FT_Library   ftLibrary;
static bool         isInit = false;

}
