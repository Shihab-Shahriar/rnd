#include <cstdint>
#include <iostream>
#include <limits>

#include "base_state.hpp"

namespace{
  static inline uint32_t rotl(uint32_t value, unsigned int x) {
    return (value << x) | (value >> (32 - x));
  }
}

class Tyche : public BaseRNG<Tyche> {
public:
  //FIXME: we have to init the seed to avoid bad cases
  Tyche(uint64_t seed, uint32_t idx) {
    a = static_cast<uint32_t>(seed >> 32);
    b = static_cast<uint32_t>(seed & 0xFFFFFFFFULL);
    d = d ^ idx;

    for (int i = 0; i < 20; i++) {
      mix();
    }
    std::cout<<std::hex<<a<<" "<<b<<" "<<c<<" "<<d<<std::endl;
  }

  template <typename T = uint32_t> DEVICE T draw() {
    mix();
    if constexpr (std::is_same_v<T, uint32_t>)
      return b;
    
    else{
      uint32_t tmp = b;
      mix();
      uint64_t res = (static_cast<uint64_t>(tmp) << 32) | b;
      return static_cast<T>(res);
    }

  }

private:
  void mix() {
    a += b;
    d = rotl(d ^ a, 16);
    c += d;
    b = rotl(b ^ c, 12);
    a += b;
    d = rotl(d ^ a, 8);
    c += d;
    b = rotl(b ^ c, 7);
  }

  uint32_t a, b;
  uint32_t c = 2654435769;
  uint32_t d = 1367130551;
};
