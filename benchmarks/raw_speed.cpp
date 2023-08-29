/*
    This compares the raw speed of all the generators on both CPU and GPU.
 */

#include <iostream>
#include <chrono>
#include <cstdint>
#include <omp.h>

#include "phillox.h"
#include "threefry.h"
#include "squares.h"
#include "tyche.h"

using std::cout;
using std::endl;

const int N = 268435456; // no of 32 bits integers required for 1 GB data

template<typename RNG>
double measure_speed(){

    using namespace std::chrono;
    RNG rng(12345, 0);

    auto start = high_resolution_clock::now();

    // sum the generated values to avoid getting the compiler to optimize the code away
    uint64_t sum = 0;
    for(int i = 0; i < N; ++i){
        sum += rng.template draw<uint32_t>();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    sum &= 1; // avoid polluting the output     

    // Total gigabytes produced
    double total_gb = N * sizeof(uint32_t) / 1e9;
    double time_taken = duration.count() / 1e6;

    //speed: GB/s
    double speed = total_gb / time_taken;

    std::cout << "Speed: " << speed << " GB/s " << sum<< std::endl;
    return duration.count();
}





template<typename RNG>
double measure_speed_openmp(){

    using namespace std::chrono;
    RNG rng(12345, 0);
    uint64_t global_sum = 0; // Global sum

    auto start = high_resolution_clock::now();

    // Parallelize the loop with OpenMP
    #pragma omp parallel
    {
        // Each thread gets its own RNG instance
        int thread_id = omp_get_thread_num();
        int local_N = N / omp_get_num_threads();
        //cout<<local_N<<endl;
        uint64_t sum = 0;
        
        RNG rng(12345 + thread_id, 0);  // Initialize with different keys

        #pragma omp for
        for(int i = 0; i < local_N; ++i){
            sum += rng.template draw<uint32_t>();
        }
        
        #pragma omp critical
        {
            global_sum += sum;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    global_sum &= 1; // avoid polluting the output     


    // Total gigabytes produced
    double total_gb = N * sizeof(uint32_t) / 1e9;
    double time_taken = duration.count() / 1e6;

    //speed: GB/s
    double speed = total_gb / time_taken;

    std::cout << "Speed: " << speed << " GB/s " << global_sum<< std::endl;
    return duration.count();
}





int main(){

    cout<<"Phillox: "<<endl;
    measure_speed<Phillox>();

    cout<<"Threefry: "<<endl;
    measure_speed<Threefry>();

    cout<<"Squares: "<<endl;
    measure_speed<Squares>();

    cout<<"Tyche: "<<endl;
    measure_speed<Tyche>();

    cout<<"Phillox OpenMP: "<<endl;
    measure_speed_openmp<Phillox>();

    cout<<"Threefry OpenMP: "<<endl;
    measure_speed_openmp<Threefry>();

    cout<<"Squares OpenMP: "<<endl;
    measure_speed_openmp<Squares>();

    cout<<"Tyche OpenMP: "<<endl;
    measure_speed_openmp<Tyche>();

    return 0;
}