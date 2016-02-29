//============================================================================
// Name        : simpleGtest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <gtest/gtest.h>

template <typename A>
A add(const A& a, const A& b)
{
  return a + b;
}

TEST(Addition, CanAddTwoNumbers)
{
  EXPECT_TRUE(add(2, 2) == 4);
}

int main(int argc, char **argv) {
  std::cout << "Hello, world\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
