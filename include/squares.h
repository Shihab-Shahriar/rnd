#include <stdint.h>

#include "base_state.hpp"


namespace {

/*
These two functions below were taken directly from the author's
website at: https://squaresrng.wixsite.com/rand
*/

#define K 0xc58efd154ce32f6d                                           

inline static uint32_t squares32(uint64_t ctr, uint64_t key) {

   uint64_t x, y, z;

   y = x = ctr * key; z = y + key;
   
   x = x*x + y; x = (x>>32) | (x<<32);       /* round 1 */

   x = x*x + z; x = (x>>32) | (x<<32);       /* round 2 */

   x = x*x + y; x = (x>>32) | (x<<32);       /* round 3 */

   return (x*x + z) >> 32;                   /* round 4 */

}

/**************************************************************************\
*                                                                          *
*  Five round counter-based middle square (64-bit output)                  *
*                                                                          *
*  squares64(ctr,key) - returns 64-bit unsigned int [0,0xffffffffffffffff] *
*                                                                          *
*  The result of the fourth round is xored into the fifth round to produce * 
*  a 64-bit output.                                                        *
*                                                                          *
\**************************************************************************/

inline static uint64_t squares64(uint64_t ctr, uint64_t key) {

   uint64_t t, x, y, z;

   y = x = ctr * key; z = y + key;
   
   x = x*x + y; x = (x>>32) | (x<<32);       /* round 1 */

   x = x*x + z; x = (x>>32) | (x<<32);       /* round 2 */

   x = x*x + y; x = (x>>32) | (x<<32);       /* round 3 */

   t = x = x*x + z; x = (x>>32) | (x<<32);   /* round 4 */

   return t ^ ((x*x + y) >> 32);             /* round 5 */

}
}

/*
In Squares, since the seed (i.e. key) has to have certain properties to 
generate good random numbers, we can't allow user to set arbritray seed.
To avoid the pitfall of weak user supplied seed, we combine that seed with
a known good key (defined above "K") by multiplying it with that key. 
*/

class Squares: public BaseRNG<Squares>{
public:
    Squares(uint64_t _seed, uint32_t _ctr)
    :  seed((_seed + 1) * K)
    {
        ctr = static_cast<uint64_t>(_ctr) << 32;
    }

    template <typename T = uint32_t> DEVICE T draw() {
        if constexpr (std::is_same_v<T, uint32_t>)
            return squares32(ctr++, seed);
        else
            return squares64(ctr++, seed);
    }

private:
    const uint64_t seed;
    uint64_t ctr = 0;
};