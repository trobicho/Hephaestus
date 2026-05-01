#pragma once

#include <cstdint>
#include <string>

namespace HephGui {

using   HephGuiId = uint64_t;

HephGuiId HephHashStr(const char* data);

}
