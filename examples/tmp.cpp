#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <cstdio>

#include "../include/tyche.h"
#include "../include/squares.h"
#include "../include/threefry.h"

using namespace std;

int main() {
  int seed = 0, ctr = 0;

  using RNG = Threefry; // Tyche


  RNG rng(seed, ctr); // rng2(seed, ctr + 1);

  for(int i=0; i<10; i++) {
        uint32_t value = rng.draw<uint32_t>();
        cout<<hex<<value<<endl;
  }
}