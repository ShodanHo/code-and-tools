/*
 * test_json_fmt.cpp
 *
 *  Created on: 18 Mar 2016
 *      Author: stuartf
 */


#include <iostream>
#include <gtest/gtest.h>
#include "json_fmt.h"
#include "timeval_utils.h"

TEST(Addition, CanAddTwoNumbers)
{
  EXPECT_EQ(JsonFmtLabelXyzValueRunningZTime(false, timeval_ctor(1,2))(),
            "{\"label\":\"xyz\",\"Value\":\"STOPPED\",\"time\":\"1.000002\"}");
  EXPECT_EQ(JsonFmtLabelXyzValueRunningZTime(true, timeval_ctor(1,2))(),
            "{\"label\":\"xyz\",\"Value\":\"RUNNING\",\"time\":\"1.000002\"}");
}

int main(int argc, char **argv)
{
  std::cout << "Hello, world\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
