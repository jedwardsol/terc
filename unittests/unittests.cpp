#include <gtest/gtest.h>

#if defined _DEBUG
#pragma comment(lib,"gtestd")
#else
#pragma comment(lib,"gtest")
#endif

#include "Arithmetic/Arithmetic.h"
#pragma comment(lib,"arithmetic")

TEST(MainTest, MainRuns) 
{
  EXPECT_EQ(true,true);
}

int main(int argc, char *argv[]) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}