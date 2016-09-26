#include <gtest/gtest.h>
#include <max_sub_sequence.hxx>

// STD includes
#include <functional>

// Testing namespace
using namespace SHA_Search;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
}
#endif /* DOXYGEN_SKIP */

// Test MaxSubSequence
TEST(TestSearch, MaxSubSequence)
{
  // Should return <5,9> (maximal sum of 17)
  {
    const Container_Type kMarketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> kMaxBeneficeIndexes =
      MaxSubSequence<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (kMarketPrices.begin(), kMarketPrices.end());
    EXPECT_EQ(5, kMaxBeneficeIndexes.first);
    EXPECT_EQ(9, kMaxBeneficeIndexes.second);
  }

  // Should return <FirstPositiveIdx, idxEnd> on sorted array
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> kIndexes =
      MaxSubSequence<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (kSortedArray.begin(), kSortedArray.end());
    EXPECT_EQ(2, kIndexes.first);
    EXPECT_EQ(static_cast<int>(kSortedArray.size()) - 1, kIndexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const Container_Type insufficientArray = Container_Type(1, 2);
    const std::pair<int, int> kIndexes =
      MaxSubSequence<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (insufficientArray.begin(), insufficientArray.end());
    EXPECT_EQ(-1, kIndexes.first);
    EXPECT_EQ(-1, kIndexes.second);
  }

  // Should return <0,1> on array containing only two positive elements
  {
    const Container_Type twoElementArray = Container_Type(2, 2);
    const std::pair<int, int> kIndexes =
      MaxSubSequence<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (twoElementArray.begin(), twoElementArray.end());
    EXPECT_EQ(0, kIndexes.first);
    EXPECT_EQ(1, kIndexes.second);
  }

  // Should return <0, idxEnd> on array containing the same positive value
  {
    const int kSize = 10;
    Container_Type sameElementArray = Container_Type(kSize, 2);
    const std::pair<int, int> indexes =
      MaxSubSequence<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (sameElementArray.begin(), sameElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(kSize - 1, indexes.second);
  }
}
