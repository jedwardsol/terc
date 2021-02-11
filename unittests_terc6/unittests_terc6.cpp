#include "googletest/gtest.h"

#pragma comment(lib,"arithmetic")
#pragma comment(lib,"architecture")


int main(int argc, char *argv[]) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}