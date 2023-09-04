#include "phillox.h"
#include <gtest/gtest.h>

using std::cout;
using std::endl;

// TODO: Add std::is_trivially_copyable and std::is_trivially_destructible tests for all generator types


TEST(RNG, basic) {
  Phillox rng(42, 0);
  EXPECT_NE(rng.rand<long long int>(), rng.rand<long long int>());

  Phillox rng1(1, 0);
  Phillox rng2(1, 0);
  EXPECT_EQ(rng1.rand<int>(), rng2.rand<int>());
  EXPECT_EQ(rng1.rand<int>(), rng2.rand<int>());

  Phillox rng3(3, 0);
  Phillox rng4(4, 0);
  EXPECT_NE(rng3.rand<int>(), rng4.rand<int>()); // this "could" happen :)
}

TEST(RNG, mean) {
  Phillox rng(42, 0);
  int num_draws = 10000;

  float mean = 0;
  for (int i = 0; i < num_draws; i++) {
    mean += rng.rand<float>();
  }
  mean /= num_draws;
  cout << mean << endl;
  EXPECT_EQ(7 * 6, 42);
}
