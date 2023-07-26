#include <cstdint>
#include <iostream>
#include <array>
#include <limits>

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



inline uint64_t mulhilo(uint32_t L, uint32_t R, uint32_t *hip) {
    uint64_t product = static_cast<uint64_t>(L)*static_cast<uint64_t>(R);
    *hip = product>>32;
    return static_cast<uint32_t>(product);
}

void bumpkey(std::array<uint32_t, 2>& key ){
    key[0] += PHILOX_W32_0;
    key[1] += PHILOX_W32_1;
}

std::array<uint32_t, 4> round(std::array<uint32_t, 2>& key, std::array<uint32_t, 4>& ctr){
    uint32_t hi0;                                                              
    uint32_t hi1;                                                              
    uint32_t lo0 = mulhilo(PHILOX_M4x32_0, ctr[0], &hi0);              
    uint32_t lo1 = mulhilo(PHILOX_M4x32_1, ctr[2], &hi1);              
    return {{hi1^ctr[1]^key[0], lo1, hi0^ctr[3]^key[1], lo0}};           
}



class Phillox{
    public:

    Phillox(uint64_t seed_, uint64_t counter_) : 
        seed(seed_), counter(counter_)
    {

    }


    std::array<uint32_t, 4> generate(){
        std::array<uint32_t, 2> key = {(uint32_t)(seed>>32), (uint32_t)(seed & 0xFFFFFFFF)}; 
        std::array<uint32_t, 4> ctr = {0, 0, (uint32_t)(counter>>32), (uint32_t)(counter & 0xFFFFFFFF)};

        for(int r=0; r<10; r++){
            if(r>0) bumpkey(key);
            ctr = round(key, ctr);
        }
        counter++;
        return ctr;
    }

    template <typename T>
    T urand(){
        std::array<uint32_t, 4> out = generate();
        if constexpr (sizeof(T)==4) return static_cast<T>(out[0]);
        else{
            T ret = ((T)out[0] << 32) | out[1];
            return ret;
        }
    }

    template <typename T>
    T draw(){
        if constexpr (std::is_integral_v<T>){
            return urand<T>();
        }
        else{
            return urand<uint64_t>() / static_cast<T>(std::numeric_limits<uint64_t>::max()); 
        }
    }

    void fill_random(int *array, int N){

    }

    uint64_t seed;
    uint64_t counter;




};
