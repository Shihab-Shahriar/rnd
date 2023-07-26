#include <cstdint>
#include <iostream>

#include "phillox.h"

using std::cout;
using std::endl;

int main(){
    Phillox rng(1, 0);

    // Draw random numbers of many types
    int a = rng.draw<int>();
    auto b = rng.draw<long long int>();
    double c = rng.draw<double>();
    float f = rng.draw<float>();

    cout<<a<<", "<<b<<" "<<c<<" "<<f<<endl;

}