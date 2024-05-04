#include <gtest/gtest.h>

#include "vector.hpp"

using ywen::vector;

TEST(Test_ywen_vector, test_constructor_empty)
{
  {
    vector<int> v;

    EXPECT_TRUE(v.empty());
    EXPECT_TRUE(0U == v.size());
    EXPECT_TRUE(0U <= v.capacity());
  }
}

TEST(Test_ywen_vector, test_constructor_initializer)
{
  {
    vector<int> v = {};

    EXPECT_TRUE(v.empty());
    EXPECT_TRUE(0U == v.size());
    EXPECT_TRUE(0U <= v.capacity());
  }

  {
    vector<int> v = {3, 2, 1};

    EXPECT_TRUE(3U == v.size());
    EXPECT_TRUE(3U <= v.capacity());

    int expected_values[3] = {3, 2, 1};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_EQ(v.at(i), expected_values[i]);
    }
  }
}

TEST(Test_ywen_vector, test_insert)
{
  {
    vector<int> v;

    v.insert(0, 10);
    EXPECT_TRUE(1U == v.size());
    EXPECT_TRUE(1U <= v.capacity());
    EXPECT_TRUE(10 == v.at(0));
  }

  {
    vector<int> v = {10, 10, 10, 10};
    v.insert(0U, 0);  // The first position (i.e., left-most boundary)

    EXPECT_TRUE(5U == v.size());

    int expected_values[5] = {0, 10, 10, 10, 10};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_TRUE(v.at(i) == expected_values[i]);
    }
  }

  {
    vector<int> v = {10, 10, 10, 10};
    v.insert(2U, 0);  // Position in the middle.

    EXPECT_TRUE(5U == v.size());

    int expected_values[5] = {10, 10, 0, 10, 10};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_TRUE(v.at(i) == expected_values[i]);
    }
  }

  {
    vector<int> v = {10, 10, 10, 10};
    v.insert(3U, 0);  // The last position (i.e., right-most boundary)

    EXPECT_TRUE(5U == v.size());

    int expected_values[5] = {10, 10, 10, 0, 10};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_TRUE(v.at(i) == expected_values[i]);
    }
  }

  {
    vector<int> v = {10, 10, 10, 10};
    v.insert(4U, 0);  // The tail (i.e., beyond the right-most boundary)

    EXPECT_TRUE(5U == v.size());

    int expected_values[5] = {10, 10, 10, 10, 0};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_TRUE(v.at(i) == expected_values[i]);
    }
  }
}

TEST(Test_ywen_vector, test_erase)
{
  {
    vector<int> v = {1};
    EXPECT_EQ(1U, v.size());

    v.erase(0U);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(0U, v.size());
  }

  {
    vector<int> v = {1, 2, 3, 4};
    EXPECT_TRUE(4U == v.size());

    v.erase(0U);
    EXPECT_TRUE(3U == v.size());
    int expected_values[3] = {2, 3, 4};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_EQ(v.at(i), expected_values[i]);
    }
  }

  {
    vector<int> v = {1, 2, 3, 4};
    EXPECT_TRUE(4U == v.size());

    v.erase(1U);
    EXPECT_TRUE(3U == v.size());
    int expected_values[3] = {1, 3, 4};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_EQ(v.at(i), expected_values[i]);
    }
  }

  {
    vector<int> v = {1, 2, 3, 4};
    EXPECT_TRUE(4U == v.size());

    v.erase(3U);
    EXPECT_TRUE(3U == v.size());
    int expected_values[3] = {1, 2, 3};
    for (size_t i = 0; i < v.size(); ++i)
    {
      EXPECT_EQ(v.at(i), expected_values[i]);
    }
  }
}

TEST(Test_ywen_vector, test_regular_use)
{
  vector<size_t> v;

  EXPECT_TRUE(v.empty());

  const size_t N = 4;

  for (size_t i = 0; i < 2; ++i)
  {
    for (size_t j = 0; j < N; ++j)
    {
      v.push_back(j);
    }
    EXPECT_EQ(N, v.size());

    for (size_t j = 0; j < N; ++j)
    {
      v.pop_back();
    }
    EXPECT_TRUE(v.empty());
  }
}
