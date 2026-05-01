#include "hashFunction.hpp"

#define FNV_offset_basis  0xcbf29ce484222325
#define FNV_prime         0x100000001b3

namespace HephGui {

HephGuiId HephHashStr(const char* data) {
  HephGuiId result = FNV_offset_basis;

  while (unsigned char c = *data++) {
    if (c == '#' && data[0] == '#' && data[1] == '#') {
      result = FNV_offset_basis;
    }

    result ^= c;
    result *= FNV_prime;
  }

  return (result);
}

}
