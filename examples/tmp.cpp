#include <array>
#include <cstdint>
#include <iostream>
#include <limits>

#include "../include/tyche.h"
#include "../include/squares.h"

using namespace std;

int main() {
  int seed = 0, ctr = 0;

  using RNG = Tyche; // Tyche


  RNG rng(seed, ctr), rng2(seed, ctr + 1);

  uint32_t q = rng.draw<uint32_t>();
  uint32_t p = q & 0xFFFF0000; 
  cout<<q<<" "<<p<<endl;

  for (int i = 0; i < 10; i++)
    cout << (rng.draw<uint32_t>() & 0xFFFF0000) << endl;
  cout << endl;

  for (int i = 0; i < 10; i++)
    cout << rng2.draw<uint32_t>() << endl;
  cout << endl;

  cout << rng.draw<uint32_t>() << endl;
  cout << rng2.draw<uint32_t>() << endl;
}