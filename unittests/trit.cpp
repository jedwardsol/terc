#include <gtest/gtest.h>


#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"
#include "Arithmetic/trit.h"


TEST(TritTest, TritTestsRun) 
{
  EXPECT_EQ(true,true);
}

TEST(TritTest, TritConstruct) 
{
    trit    t0{0};
    trit    t1{1};
    trit    tm{-1};

    EXPECT_EQ(t0.t,0);
    EXPECT_EQ(t1.t,1);
    EXPECT_EQ(tm.t,-1);
}

TEST(TritTest, TritBadConstruct) 
{
    auto construct = [](int i)
    {
        trit t{i};
    };

    EXPECT_THROW(construct( 2),std::out_of_range);
    EXPECT_THROW(construct(-2),std::out_of_range);
}

