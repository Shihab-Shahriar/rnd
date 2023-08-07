#include <cstdio>
#include <cstdint>

#include "../include/tyche.h"
#include "../include/squares.h"
#include "../include/phillox.h"


int main()
{
    using RNG = Squares;


    RNG rng(0, 0);

    while (1) {
        uint32_t value = rng.draw<uint32_t>();
        fwrite((void*) &value, sizeof(value), 1, stdout);
    }
}