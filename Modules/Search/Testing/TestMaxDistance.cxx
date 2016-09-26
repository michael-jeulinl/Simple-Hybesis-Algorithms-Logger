#include <gtest/gtest.h>
#include <max_distance.hxx>

// STD includes
#include <functional>

// Testing namespace
using namespace SHA_Search;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
}
#endif /* DOXYGEN_SKIP */

// Test SimpleStockMarketProblem on a simple market array
TEST(TestSearch, MaxDistances)
{
  // Should return <4,9> (largest benefice of 6)
  {
    const Container_Type marketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> maxBeneficeIndexes =
      MaxDistance<Const_Iterator_Type, std::minus<int>>(marketPrices.begin(), marketPrices.end());
    EXPECT_EQ(4, maxBeneficeIndexes.first);
    EXPECT_EQ(9, maxBeneficeIndexes.second);
  }

  // Should return <0, idxEnd> on sorted array
  {
    const Container_Type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> indexes =
      MaxDistance<Const_Iterator_Type, std::minus<int>>(sortedArray.begin(), sortedArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(static_cast<int>(sortedArray.size()) - 1, indexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const Container_Type insufficientArray = Container_Type(1, 2);
    const std::pair<int, int> indexes =
      MaxDistance<Const_Iterator_Type, std::minus<int>>(insufficientArray.begin(), insufficientArray.end());
    EXPECT_EQ(-1, indexes.first);
    EXPECT_EQ(-1, indexes.second);
  }

  // Should return <0,1> on array containing only two elements
  {
    const Container_Type twoElementArray = Container_Type(2, 2);
    const std::pair<int, int> indexes =
      MaxDistance<Const_Iterator_Type, std::minus<int>>(twoElementArray.begin(), twoElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // Should return <0,1> on array containing the same value
  {
    const Container_Type sameElementArray = Container_Type(10, 2);
    const std::pair<int, int> indexes =
      MaxDistance<Const_Iterator_Type, std::minus<int>>(sameElementArray.begin(), sameElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // String - should return <1,6> as 'a', 'z' are the most distanced letter
  {
    const std::pair<int, int> indexes =
      MaxDistance<std::string::const_iterator, std::minus<char>>(RandomStr.begin(), RandomStr.end());
    EXPECT_EQ(1, indexes.first);
    EXPECT_EQ(6, indexes.second);
  }
}
