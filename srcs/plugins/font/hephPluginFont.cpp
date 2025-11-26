#include "hephPluginFont.hpp"
#include <sstream>
#include <string>

namespace HephPluginFont {

static FT_Library   ftLibrary;
static bool         isInit = false;

FT_Library  getLib(){
  return (ftLibrary);
}

HephResult  ftError(FT_Error error) {
  const char*         errorPtr = FT_Error_String(error);
  std::stringstream   errorStream;

  if (errorPtr == nullptr) {
    errorStream << std::hex << error;
  }
  else
    errorStream << errorPtr;
  return (HephResult(errorStream.str(), error == 0));
}

HephResult  init() {
  if (isInit)
    return (HephResult());
  FT_Error  error = FT_Init_FreeType(&ftLibrary);
  if (error) {
    return (ftError(error).errorFormat("failed to initialize FreeType library {{}} !"));
  }
  isInit = true;
  return (HephResult());
}

HephResult  checkInit() {
  return (HephResult("HephPluginFont as not been initialized (call HephPluginFont::init())", isInit));
}

}
