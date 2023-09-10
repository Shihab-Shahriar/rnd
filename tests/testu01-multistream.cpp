/**
 * @file 
 * @brief Simplified version of the multi-stream correlation test outlined in [1]. 
 * 
 * We hold either key or counter constant, and increment other by 1. This tests 
 * both multi-stream and sub-streams approaches of creating mulptiple streams [1].
 * Although simplified, this closely resembles how we expect these parameters to 
 * be used in real-world parallel programs.
 * 
 * Based on: https://www.pcg-random.org/posts/how-to-test-with-testu01.html
 * 
 * [1] Salmon, John K., et al. "Parallel random numbers: as easy as 1, 2, 3." 
 * Proceedings of 2011 international conference for high performance computing, 
 * networking, storage and analysis. 2011.
 */

#include <random>
#include <string>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <algorithm>

extern "C" {
    #include "TestU01.h"
}

#include "../include/tyche.h"
#include "../include/squares.h"
#include "../include/phillox.h"


// Control parameters for this test program.
const char* gen_name = "Phillox";  
using RNG = Phillox;
const int C = 3; // C numbers from per stream
const int NS = 100; // from NS streams
// end

std::vector<uint32_t> buffer;
std::vector<RNG> generators;

// We keep either key or counter constant, and increment other by 1
void init_generators(bool k_stride = 0, bool c_stride = 1){
    for(int i = 0; i < NS; i++){
        generators.emplace_back(i*k_stride, i*c_stride);
    }
}

void populate_buffer(){
    for(int i = 0; i < NS; i++){
        for(int j = 0; j < C; j++){
            buffer.push_back(generators[i].draw<uint32_t>());
        }
    }
    std::reverse(buffer.begin(), buffer.end());
}

uint32_t gen32()
{
    if(buffer.empty()){
        populate_buffer();
    }
    uint32_t res = buffer.back();
    buffer.pop_back();
    return res;
}

// END OF GENERATOR SECTION

inline uint32_t rev32(uint32_t v)
{
    // https://graphics.stanford.edu/~seander/bithacks.html
    // swap odd and even bits
    v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
    // swap consecutive pairs
    v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
    // swap nibbles ...
    v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
    // swap bytes
    v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
    // swap 2-byte-long pairs
    v = ( v >> 16             ) | ( v               << 16);
    return v;
}

uint32_t gen32_rev()
{
    return rev32(gen32());
}


int main ()
{
    // Config options for generator output
    bool reverseBits = false;

    // Name of the generator

    std::string genName = gen_name;

    printf("Testing %s:\n", genName.c_str());
    fflush(stdout);

    // Create multiple streams. Here, seed=0, counter is incremented.
    init_generators(0, 1);

    // Create multiple streams. Here, counter=0, seed is incremented.
    //init_generators(1, 0);

    // Create a generator for TestU01.
    unif01_Gen* gen =
        unif01_CreateExternGenBits((char*) genName.c_str(),
                                   reverseBits ? gen32 : gen32_rev);

    // Run tests.

    // bbattery_SmallCrush(gen);
    // fflush(stdout);
    
    bbattery_Crush(gen);
    fflush(stdout);
    
    //bbattery_BigCrush(gen);
    //fflush(stdout);

    // Clean up.
    unif01_DeleteExternGenBits(gen);

    return 0;
}