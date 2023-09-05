#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstdint>

#ifdef __CUDA_ARCH__
  #define DEVICE __host__ __device__
#elif defined(__HIP_DEVICE_COMPILE__)
  #define DEVICE __device__ __host__
#else
  #define DEVICE 
#endif


namespace rnd{

// NOTE: nvcc compiler replaces floating point variants with cuda built-in versions
// NOTE: floating point variants are not part of std namespace for some reason

template <typename T> 
inline DEVICE T sin(T x) {
    if constexpr (std::is_same_v<T, float>)
        return sinf(x);
    else if constexpr (std::is_same_v<T, double>)
        return std::sin(x);
}

template <typename T>
inline DEVICE T cos(T x) {
    if constexpr (std::is_same_v<T, float>)
        return cosf(x);
    else if constexpr (std::is_same_v<T, double>)
        return std::cos(x);
}

template <typename T>
inline DEVICE T log(T x) {
    if constexpr (std::is_same_v<T, float>)
        return logf(x);
    else if constexpr (std::is_same_v<T, double>)
        return std::log(x);
}

template <typename T>
inline DEVICE T sqrt(T x) {
    if constexpr (std::is_same_v<T, float>)
        return sqrtf(x);
    else if constexpr (std::is_same_v<T, double>)
        return std::sqrt(x);
}

}




#endif
