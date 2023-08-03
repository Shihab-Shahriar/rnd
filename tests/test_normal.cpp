#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>

#include "phillox.h"
#include "tyche.h"

using std::cout;
using std::endl;

int main(){
    using RNG=Phillox;
    RNG rng(42, 0);
    int num_draws = 100000;

    for (int i = 0; i < num_draws-1; i++) printf("%f ", rng.randn<float>());
    printf("%f", rng.randn<float>());
    return 0;
}

