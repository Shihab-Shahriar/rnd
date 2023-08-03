## 07/27/2023

### Current State
+ Implementation of Philox Generator.
    + ready for Mundy.
+ Code works in both CPU & GPU
    + around 50% faster than Kokkos on GPU for 8 million numbers.
+ Passes preliminary tests based on google test.
+ No dependency on Kokkos or any other library.

### Todo
+ Add some more counter-based random number generators (threefry for example).
+ Implement splittable random number generator for dynamic creation of rng on device [1]. (could be a way to assign unique global id to new particles).
+ Add statistical tests from Test01 suite [2]. 
+ Write documentation, and optionally host it. (Maybe github wiki for now)
+ Create CI/CD pipeline.



[1] Claessen, Koen, and Michał H. Pałka. "Splittable pseudorandom number generators using cryptographic hashing." ACM SIGPLAN Notices 48, no. 12 (2013): 47-58.

[2] L'ecuyer, Pierre, and Richard Simard. "TestU01: AC library for empirical testing of random number generators." ACM Transactions on Mathematical Software (TOMS) 33, no. 4 (2007): 1-40.


## 08/03/2023

### Current State
+ Added one more generator (Tyche).
+ Added more distributions (normal and gamma).
+ Refactored the code using CRTP- a base RNG class to inject common code in all generators.
+ Added examples:
    + Basic Usage, How to generate PI in parallel
+ Few more tests. 


### TODO
+ A writeup on literature review.
+ Add threfry generator. 
+ Add some statistical tests from Test01 suite 
    + Test all generators using the "big crush" battery once done.
+ Documentation, CI pipeline.