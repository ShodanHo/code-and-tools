#include <iostream>
#include <gtest/gtest.h>

unsigned wrapAroundArray(unsigned i, unsigned N)
{
  // "wrap" i around ThisArray, landing on a valid index
  while (i >= N) { i -= N; }
  return i;
}

struct InOutWrapping
{
private:
  const unsigned N;
  unsigned in, out;
public:
  //InOutWrapping(T(&ThisArray)[_N]) : N(_N), in(0), out(0) {}
  InOutWrapping(unsigned _N) : N(_N), in(0), out(0) {}
  bool full() { return wrapAroundArray(in + 1,N) == out; }
  bool empty() { return in == out; }
  unsigned inAdvance() { unsigned ret = in; in = wrapAroundArray(in + 1,N); return ret; }
  unsigned outAdvance() { unsigned ret = out; out = wrapAroundArray(out + 1,N); return ret; }
};

TEST(InOutWrapping, inAdvanceWrapping)
{
  InOutWrapping indices(3);
  EXPECT_TRUE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.inAdvance() == 0);
  EXPECT_FALSE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.inAdvance() == 1);
  EXPECT_FALSE(indices.empty());
  EXPECT_TRUE(indices.full());
  EXPECT_TRUE(indices.inAdvance() == 2);
  EXPECT_TRUE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.inAdvance() == 0);
  EXPECT_FALSE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.inAdvance() == 1);
  EXPECT_FALSE(indices.empty());
  EXPECT_TRUE(indices.full());
  EXPECT_TRUE(indices.inAdvance() == 2);
  EXPECT_TRUE(indices.empty());
  EXPECT_FALSE(indices.full());
}

TEST(InOutWrapping, outAdvanceWrapping)
{
  InOutWrapping indices(3);
  EXPECT_TRUE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.outAdvance() == 0);
  EXPECT_FALSE(indices.empty());
  EXPECT_TRUE(indices.full());
  EXPECT_TRUE(indices.outAdvance() == 1);
  EXPECT_FALSE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.outAdvance() == 2);
  EXPECT_TRUE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.outAdvance() == 0);
  EXPECT_FALSE(indices.empty());
  EXPECT_TRUE(indices.full());
  EXPECT_TRUE(indices.outAdvance() == 1);
  EXPECT_FALSE(indices.empty());
  EXPECT_FALSE(indices.full());
  EXPECT_TRUE(indices.outAdvance() == 2);
}

int main(int argc, char **argv) {
  std::cout << "Hello, world\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
