#include <cstdint>
#include <iostream>
#include <vector>

#include "phillox.h"

using std::cout;
using std::endl;

struct Particle{
    const int global_id;
    int counter = 0; 
    double pos[3];

    Particle (int id) : global_id(id) {}
};

int main(){
    Phillox rng(1, 0);

    // Draw random numbers of many types
    int a = rng.draw<int>();
    auto b = rng.draw<long long int>();
    double c = rng.draw<double>();
    float f = rng.draw<float>();

    cout<<a<<", "<<b<<" "<<c<<" "<<f<<endl;

    // Create independent streams of numbers in parallel
    float data[16][10];

    #pragma omp parallel for 
    for(int i=0; i<16; i++){
        Phillox rng(i,0);
        for(int j=0; j<10; j++) data[i][j] = rng.draw<float>();
    }

    for(int i=0; i<16; i++){
        for(int j=0; j<10; j++) cout<<data[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;

    // How to use a unique, independent RNG for each particle in a simulation- 
    // The key is to maintain a counter variable for each particle, and 
    // increment it each time the rng is instantiated. 
    std::vector<Particle> system; 
    for(int i=0; i<16; i++) system.emplace_back(i);

    // initialize
    #pragma omp parallel for 
    for(int i=0; i<16; i++){
        Particle& p = system[i];
        // if you don't increment p.counter here, you're going to get exactly 
        // same values in next loop. 
        Phillox rng1(p.global_id,p.counter++);
        for(int j=0; j<3; j++) p.pos[j] = rng.draw<double>();
    }

    // a random step
    #pragma omp parallel for 
    for(int i=0; i<16; i++){
        Particle& p = system[i];
        Phillox rng2(p.global_id,p.counter++);
        for(int j=0; j<3; j++) p.pos[j] += rng.draw<double>() / 10;
    }

    for(int i=0; i<16; i++){
        Particle& p = system[i];
        for(int j=0; j<3; j++) cout<<p.pos[j]<<" ";
        cout<<p.counter<<endl;
    }

    return 0;
}