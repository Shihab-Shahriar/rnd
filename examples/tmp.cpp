#include <array>
#include <cstdint>
#include <iostream>
#include <limits>

#include "../include/tyche.h"

using namespace std;

int main() {
  int seed = 1, idx = 0;
  Tyche rng(seed, idx), rng2(seed, idx + 1);

  for (int i = 0; i < 10; i++)
    cout << rng.rand() << endl;
  cout << endl;
  for (int i = 0; i < 10; i++)
    cout << rng2.rand() << endl;
  cout << endl;

  cout << rng.rand() << endl;
  cout << rng2.rand() << endl;

  cout << endl;
  float b[10];
  rng.fill_random(b, 10);
  for (int i = 0; i < 10; i++)
    cout << b[i] << endl;
}