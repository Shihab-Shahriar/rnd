#include <cuda_runtime.h>
#include <iostream>
#include <chrono>

#include "phillox.h"
#include "threefry.h"
#include "squares.h"
#include "tyche.h"

using std::cout;
using std::endl;

const int N = 268435456; // no of 32 bits integers required for 1 GB data

template<typename RNG>
__global__ void measure_speed_cuda_kernel(uint32_t *global_sum_dev, int N) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    uint32_t sum = 0;

    // Assuming the RNG class has been defined to work in device code
    RNG rng(12345 + idx, 0);

    for (int i = idx; i < N; i += stride) {
        sum += rng.template draw<uint32_t>();
    }

    // Atomic add to the global sum
    atomicAdd(global_sum_dev, sum);
}


template<typename RNG>
double measure_speed_cuda() {
    using namespace std::chrono;
    uint32_t global_sum = 0;
    uint32_t *global_sum_dev;

    // Allocate memory for global_sum on the device
    cudaMalloc((void **)&global_sum_dev, sizeof(uint32_t));
    cudaMemcpy(global_sum_dev, &global_sum, sizeof(uint32_t), cudaMemcpyHostToDevice);

    auto start = high_resolution_clock::now();

    // Launch the CUDA Kernel
    int numBlocks = 256;
    int numThreadsPerBlock = 256;
    measure_speed_cuda_kernel<RNG><<<numBlocks, numThreadsPerBlock>>>(global_sum_dev, N);

    cudaDeviceSynchronize();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cudaMemcpy(&global_sum, global_sum_dev, sizeof(uint32_t), cudaMemcpyDeviceToHost);

    cudaFree(global_sum_dev);

    global_sum &= 1;  // avoid polluting the output

    // Total gigabytes produced
    double total_gb = N * sizeof(uint32_t) / 1e9;
    double time_taken = duration.count() / 1e6;

    // Speed: GB/s
    double speed = total_gb / time_taken;

    std::cout << "Speed: " << speed << " GB/s " << global_sum << std::endl;
    return duration.count();
}

int main(){

    cout<<"Phillox: "<<endl;
    measure_speed_cuda<Phillox>();

    cout<<"Threefry: "<<endl;
    measure_speed_cuda<Threefry>();

    cout<<"Squares: "<<endl;
    measure_speed_cuda<Squares>();

    cout<<"Tyche: "<<endl;
    measure_speed_cuda<Tyche>();

    return 0;
}