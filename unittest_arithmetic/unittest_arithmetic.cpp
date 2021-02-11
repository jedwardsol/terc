#include "googletest/gtest.h"

#pragma comment(lib,"arithmetic")


int main(int argc, char *argv[]) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}