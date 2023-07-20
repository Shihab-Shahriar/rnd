## Performance Portable Random Number Generator

### Design Aims:
+ Portable- Works across CPUs, all GPUs
+ Deterministic- random number generation in predictable way in all devices.
+ Parallel- capable of taking maximum advantage across all platforms.
+ Reliable- Unit tests and statistical tests to ensure algorithmic correctness and statistical soundness.


## API
```
// Current Kokkos
void func(int i) const {
    generator_type rng = rand_pool.get_state();
    int x = rng.urand64();
    ....
    rand_pool.free_state(rng);
}
```

```
// Option 1: No explicit access to state
void func(int i) const {
    int x = rand_pool.urand64();
    ....
}
```
```
// Option 2: Unique stream for each _particle_. No need to store a generator 
// with particle. Good enough for Mundy's purpose.
void func(int i) const {
    uint64_t key = particles[i].global_id.
    // get the next random number from the unique stream seeded by key
    double x = rand_pool.get_next_from_stream<double>(key); 
    double y = rand_pool.get_next_from_stream<double>(key); 
    ....
}
```
```
// Option 3: Gives most control to user. User needs to store a counter
// for each particle. This is the Random123's simplified interface. 
void func(int i) const {
    uint64_t key = particles[i].global_id.
    uint64_t counter = particles[i].counter.
    int x = rand_pool.urand64(key, counter++);
    int y = rand_pool.urand64(key, counter++);
    ....
}
```