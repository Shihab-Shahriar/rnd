#include <gtest/gtest.h>
#include "phillox.h"

using std::cout;
using std::endl;

TEST(RNG, basic) {
    Phillox rng(42, 0);
    EXPECT_NE(rng.draw(), rng.draw());

    Phillox rng1(1, 0);
    Phillox rng2(1, 0);
    EXPECT_EQ(rng1.draw(),rng2.draw());
    EXPECT_EQ(rng1.draw(),rng2.draw());

    Phillox rng3(3, 0);
    Phillox rng4(4, 0);
    EXPECT_NE(rng3.draw(), rng4.draw()); //this "could" happen :)
}

TEST(RNG, mean) {
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

