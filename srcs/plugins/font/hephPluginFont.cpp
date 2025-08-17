#include "hephPluginFont.hpp"

namespace HephPluginFont {

HephResult  ftError(FT_Error error) {
  return (HephResult(FT_Error_String(error), error == 0));
}

HephResult  init() {
  FT_Error  error = FT_Init_FreeType(&ftLibrary);
  if (error)
    return (HephResult(FT_Error_String(error)).errorFormat("failed to initialize FreeType library {{}} !"));
  isInit = true;
  return (HephResult());
}

HephResult  checkInit() {
  return (HephResult("HephPluginFont as not been initialize (call HephPluginFont::init())", isInit));
}

}
