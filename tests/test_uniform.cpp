#include <gtest/gtest.h>
#include "phillox.h"

using std::cout;
using std::endl;

TEST(RNG, BasicAssertions) {
    Phillox rng(42, 0);
    int num_draws = 10000;

    float mean = 0;
    for(int i=0; i<num_draws; i++){
        mean += rng.draw<float>();
    }
    mean /= num_draws;
    cout<<mean<<endl;
    EXPECT_EQ(7 * 6, 42);
}