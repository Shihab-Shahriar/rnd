#include <cstdint>
#include <iostream>
#include <array>
#include <limits>

#define DEVICE __host__ __device__

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


struct Counter{
    uint32_t v[4];

    DEVICE Counter(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
    
    {
        v[0] = a;
        v[1] = b;
        v[2] = c;
        v[3] = d;
    }

    DEVICE uint32_t& operator[](int i) {
        return v[i];
    }
};


struct Key{
    uint32_t v[2];

    DEVICE Key(uint32_t a, uint32_t b){
        v[0] = a;
        v[1] = b;
    }

    DEVICE uint32_t& operator[](int i) {
        return v[i];
    }
};


inline DEVICE uint64_t mulhilo(uint32_t L, uint32_t R, uint32_t *hip) {
    uint64_t product = static_cast<uint64_t>(L)*static_cast<uint64_t>(R);
    *hip = product>>32;
    return static_cast<uint32_t>(product);
}

DEVICE void bumpkey(Key& key ){
    key[0] += PHILOX_W32_0;
    key[1] += PHILOX_W32_1;
}

DEVICE Counter round(Key& key, Counter& ctr){
    uint32_t hi0;                                                              
    uint32_t hi1;                                                              
    uint32_t lo0 = mulhilo(PHILOX_M4x32_0, ctr[0], &hi0);              
    uint32_t lo1 = mulhilo(PHILOX_M4x32_1, ctr[2], &hi1);              
    return Counter(hi1^ctr[1]^key[0], lo1, hi0^ctr[3]^key[1], lo0);           
}


class Phillox{
    public:

    DEVICE Phillox(uint64_t seed_, uint64_t counter_) : 
        seed(seed_), counter(counter_)
    {

    }


    DEVICE Counter generate(){
        Key key{(uint32_t)(seed>>32), (uint32_t)(seed & 0xFFFFFFFF)}; 
        // use an internal counter for first two numbers
        Counter ctr{0, 0, (uint32_t)(counter>>32), (uint32_t)(counter & 0xFFFFFFFF)};

        for(int r=0; r<10; r++){
            if(r>0) bumpkey(key);
            ctr = round(key, ctr);
        }
        counter++;
        return ctr;
    }

    template <typename T>
    DEVICE T urand(){
        Counter out = generate();
        if constexpr (sizeof(T)==4) return static_cast<T>(out[0]);

        T ret = ((T)out[0] << 32) | out[1];
        return ret;
    }

    template <typename T = int>
    DEVICE T draw(){
        if constexpr (std::is_integral_v<T>){
            return urand<T>();
        }
        else{
            return urand<uint64_t>() / static_cast<T>(std::numeric_limits<uint64_t>::max()); 
        }
    }

    DEVICE void fill_random(int *array, int N){

    }

    uint64_t seed;
    uint64_t counter;
};
