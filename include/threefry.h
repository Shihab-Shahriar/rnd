#ifndef THREEFRY_H
#define THREEFRY_H

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>

#include "base_state.hpp"

namespace {

#define NROUNDS 20
#define SKEIN_KS_PARITY32         0x1BD11BDA


DEVICE uint32_t rotl32(uint32_t x, uint32_t N){
    return (x << (N & 31)) | (x >> ((32-N) & 31));
}

struct Key2 {
  uint32_t v[2];

  DEVICE Key2(uint32_t a, uint32_t b) {
    v[0] = a;
    v[1] = b;
  }

  DEVICE uint32_t &operator[](int i) { return v[i]; }

  DEVICE uint32_t operator[](int i) const { return v[i]; }
};


}

class Threefry: public BaseRNG<Threefry>{
public:

    DEVICE Threefry(uint64_t _seed, uint32_t _ctr)
    : seed(_seed),
      counter(static_cast<uint64_t>(_ctr) << 32)
    {
    }

    template <typename T = uint32_t> DEVICE T draw() {
        auto out = round();

        static_assert(std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>);
        if constexpr (std::is_same_v<T, uint32_t>)
            return out[0];

        uint64_t res =
            (static_cast<uint64_t>(out[0]) << 32) | static_cast<uint64_t>(out[1]);
        return static_cast<uint64_t>(res);
    }

private:

    DEVICE Key2 round(){
        uint32_t x0, x1;
        uint32_t ks0, ks1, ks2 = SKEIN_KS_PARITY32;
        ks0 = (seed >> 32);                                                       
        x0  = counter + ks0;                                                  
        ks2 ^= ks0; 

        ks1 = static_cast<uint32_t>(seed);                                                       
        x1  = _ctr + ks1;                                                  
        ks2 ^= ks1;

        for(int i=0; i<NROUNDS; i++){
            x0 += x1;
            x1 = rotl32(x1, get_constant(i%8));
            x1 ^= x0;

            if(i==3){
                x0 += ks1;
                x1 += ks2;
                x1 += 1;
            }
            if(i==7){
                x0 += ks2;
                x1 += ks0;
                x1 += 2;
            }
            if(i==11){
                x0 += ks0;
                x1 += ks1;
                x1 += 3;
            }
            if(i==15){
                x0 += ks1;
                x1 += ks2;
                x1 += 4;
            }
            if(i==19){
                x0 += ks2;
                x1 += ks0;
                x1 += 5;
            }
        } 
        _ctr++;
        return Key2(x0, x1);

    }


    DEVICE int get_constant(const int index) const {
        switch (index) {
            case 0: return 13;
            case 1: return 15;
            case 2: return 26;
            case 3: return 6;
            case 4: return 17;
            case 5: return 29;
            case 6: return 16;
            case 7: return 24;
            default: return 24;
        }
    }

    //TODO: Optimize away the internal counter. Use one counter state. 
    const uint64_t seed;
    const uint32_t counter;
    uint32_t _ctr = 0;

};

#endif // THREEFRY_H