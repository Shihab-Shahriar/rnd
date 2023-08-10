#ifndef BASE_STATE_H
#define BASE_STATE_H

#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>
#include <cmath>


#ifdef __CUDA_ARCH__
  #define DEVICE __host__ __device__
#elif defined(__HIP_DEVICE_COMPILE__)
  #define DEVICE __device__ __host__
#else
  #define DEVICE 
#endif


/*
RNG is the random number generator class. It must contain two public methods,
a constructor and a templated draw() function that returns the next 32 or 64
bit random unsigned int from its stream.
*/
template <typename RNG> class BaseRNG {
public:
  template <typename T = float> 
  DEVICE T rand() {
    if constexpr (std::is_integral_v<T>) {
      if constexpr (sizeof(T) <= 4) {
        auto x = gen().template draw<uint32_t>();
        return static_cast<T>(x);
      } else
        return static_cast<T>(gen().template draw<uint64_t>());
    } else {
      return static_cast<T>(gen().template draw<uint64_t>()) /
             static_cast<T>(std::numeric_limits<uint64_t>::max());
    }
  }

  template <typename T = float> DEVICE void fill_random(T *array, const int N) {
    for (int i = 0; i < N; i++)
      array[i] = rand<T>();
  }

  template <typename T = float> DEVICE T randn() {
    // Implements marsaglia Polar method
    // TODO: we can generate two numbers here instead of one
    static_assert(std::is_floating_point_v<T>);

    while (true) {
      auto U = 2.0 * rand<T>() - 1.0;
      auto V = 2.0 * rand<T>() - 1.0;
      auto S = U * U + V * V;
      if (S <= 1.0)
        return U * std::sqrt(-2.0 * std::log(S) / S);
    }
  }

  template <typename T = float>
  DEVICE T randn(const double &mean, const double &std_dev = 1.0) {
    return mean + randn<T>() * std_dev;
  }

  // https://www.hongliangjie.com/2012/12/19/how-to-generate-gamma-random-variables/
  template<typename T=float>
    DEVICE inline T gamma(T alpha, T b){
        T d = alpha - (1./3.);
        T c = 1. / std::sqrt(9. * d);
        T v, x;
        while(true){
            do{
                x = randn<T>();
                v = 1.0 + c * x;
            }
            while (v <= 0.);
            v = v*v*v;
            T u = rand<T>();

            const T x2 = x*x;
            if (u < 1.0 - 0.0331 * x2 *x2) return (d * v * b);

            if (std::log(u) < 0.5 * x2 + d * (1.0 - v + std::log(v))) 
                return (d * v * b);
        }
    }

private:
  DEVICE RNG &gen() { return *static_cast<RNG *>(this); }
};

#endif