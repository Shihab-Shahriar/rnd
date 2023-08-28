/*
    This compares the raw speed of all the generators on both CPU and GPU.
 */

#include <iostream>
#include <chrono>
#include <cstdint>

#include "philox.h"
#include "threefry.h"
#include "squares.h"
#include "tyche.h"

using std::cout;
using std::endl;

template<typename RNG>
double measure_speed(){

    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    // sum the generated values to avoid getting the compiler to optimize the code away
    uint64_t sum = 0;
    for(int i = 0; i < 100000000; ++i){
        sum += rng.draw<uint32_t>();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Total gigabytes produced
    double total_gb = 100000000 * sizeof(uint32_t) / 1e9;

    //speed: GB/s
    double speed = total_gb / duration.count() * 1000;

    std::cout << "Speed: " << speed << " GB/s" << std::endl;
    return duration.count();
}

int main(){

    cout<<"Phillox: "<<endl;
    measure_speed<Philox>();

    cout<<"Threefry: "<<endl;
    measure_speed<Threefry>();

    cout<<"Squares: "<<endl;
    measure_speed<Squares>();

    cout<<"Tyche: "<<endl;
    measure_speed<Tyche>();
    return 0;
}