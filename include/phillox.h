#ifndef PHILLOX_H
#define PHILLOX_H

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>

#include "base_state.hpp"

namespace {

#ifndef PHILOX_M4x32_0
#define PHILOX_M4x32_0 ((uint32_t)0xD2511F53)
#endif
#ifndef PHILOX_M4x32_1
#define PHILOX_M4x32_1 ((uint32_t)0xCD9E8D57)
#endif

#ifndef PHILOX_W32_0
#define PHILOX_W32_0 ((uint32_t)0x9E3779B9)
#endif
#ifndef PHILOX_W32_1
#define PHILOX_W32_1 ((uint32_t)0xBB67AE85)
#endif


// TODO: Move Counter & Key to util.h
struct Counter {
  uint32_t v[4];

  DEVICE Counter(uint32_t a, uint32_t b, uint32_t c, uint32_t d)

  {
    v[0] = a;
    v[1] = b;
    v[2] = c;
    v[3] = d;
  }

  DEVICE uint32_t &operator[](int i) { return v[i]; }

  DEVICE uint32_t operator[](int i) const { return v[i]; }
};

struct Key {
  uint32_t v[2];

  DEVICE Key(uint32_t a, uint32_t b) {
    v[0] = a;
    v[1] = b;
  }

  DEVICE uint32_t &operator[](int i) { return v[i]; }

  DEVICE uint32_t operator[](int i) const { return v[i]; }
};

inline DEVICE uint32_t mulhilo(uint32_t L, uint32_t R, uint32_t *hip) {
  uint64_t product = static_cast<uint64_t>(L) * static_cast<uint64_t>(R);
  *hip = product >> 32;
  return static_cast<uint32_t>(product);
}

DEVICE void bumpkey(Key &key) {
  key[0] += PHILOX_W32_0;
  key[1] += PHILOX_W32_1;
}

DEVICE Counter round(Key &key, Counter &ctr) {
  uint32_t hi0;
  uint32_t hi1;
  uint32_t lo0 = mulhilo(PHILOX_M4x32_0, ctr[0], &hi0);
  uint32_t lo1 = mulhilo(PHILOX_M4x32_1, ctr[2], &hi1);
  return Counter(hi1 ^ ctr[1] ^ key[0], lo1, hi0 ^ ctr[3] ^ key[1], lo0);
}
} // namespace


/**
 * @class Phillox
 * @brief Phillox generator
 * @note This is a modified version of Phillox generator from Random123 library.
 */
class Phillox : public BaseRNG<Phillox> {

public:
  DEVICE Phillox(uint64_t seed_, uint64_t counter_)
      : seed_hi((uint32_t)(seed_ >> 32)), 
        seed_lo((uint32_t)(seed_ & 0xFFFFFFFF)),
        counter(counter_) {}


  template <typename T = uint32_t> DEVICE T draw() {
    auto out = generate();

    static_assert(std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>);
    if constexpr (std::is_same_v<T, uint32_t>)
      return out[0];

    uint64_t res =
        (static_cast<uint64_t>(out[0]) << 32) | static_cast<uint64_t>(out[1]);
    return static_cast<uint64_t>(res);
  }

private:
  DEVICE Counter generate() {
    Key key{seed_hi, seed_lo};
    // The counter takes first 2 values from internal counter, next 2 from
    // what the user provided during instantiation. The internal counter helps
    // to avoid forcing user to increment counter each time a number is
    // generated.
    Counter ctr{(uint32_t)(_ctr >> 32), (uint32_t)(_ctr & 0xFFFFFFFF),
                (uint32_t)(counter >> 32), (uint32_t)(counter & 0xFFFFFFFF)};
    //printf("qoo\n");
    for (int r = 0; r < 10; r++) {
      if (r > 0)
        bumpkey(key);
      ctr = round(key, ctr);
    }
    _ctr++;
    return ctr;
  }

  //TODO: Optimize away the internal counter. Use one counter state. 
  const uint32_t seed_hi, seed_lo;
  const uint64_t counter;
  // private counter to keep track of numbers generated by this instance of rng
  uint64_t _ctr = 0;
};



#endif // PHILLOX_H