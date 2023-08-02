#ifndef BASE_STATE_H
#define BASE_STATE_H

#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>

#define DEVICE

/*
RNG is the random number generator class. It must contain two public methods,
a constructor and a templated draw() function that returns the next 32 or 64
bit random unsigned int from its stream.
*/
template <typename RNG> class BaseRNG {
public:
  template <typename T = float> DEVICE T rand() {
    if constexpr (std::is_integral_v<T>) {
      if constexpr (sizeof(T) <= 4) {
        auto x = gen().template draw<uint32_t>();
        return static_cast<T>(x);
      } else
        return static_cast<T>(gen().template draw<uint64_t>());
    } else {
      return gen().template draw<uint64_t>() /
             static_cast<T>(std::numeric_limits<uint64_t>::max());
    }
  }

  template <typename T = float> DEVICE void fill_random(T *array, int N) {
    for (int i = 0; i < N; i++)
      array[i] = rand<T>();
  }

private:
  RNG &gen() { return *static_cast<RNG *>(this); }
};

#endif