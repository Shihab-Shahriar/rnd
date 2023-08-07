#include <array>
#include <cstdint>
#include <iostream>
#include <limits>

#include "base_state.hpp"

namespace {

#define NROUNDS 20
#define SKEIN_KS_PARITY32         0x1BD11BDA

const uint32_t R32x2[8] = {13, 15, 26, 6, 17, 29, 16, 24};


DEVICE uint32_t rotl32(uint32_t x, uint32_t N){
    return (x << (N & 31)) | (x >> ((32-N) & 31));
}

}

class Threefry: public BaseRNG<Threefry>{
public:

    Threefry(uint64_t _seed, uint32_t _ctr)
    : seed(_seed),
      counter(static_cast<uint64_t>(_ctr) << 32)
    {
    }


private:

    DEVICE void round(){
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
            x1 = rotl32(x1, R32x2[i%8]);
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

    }

    //TODO: Optimize away the internal counter. Use one counter state. 
    const uint64_t seed;
    const uint32_t counter;
    uint32_t _ctr = 0;

};
