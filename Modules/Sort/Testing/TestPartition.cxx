#include <gtest/gtest.h>
#include <partition.hxx>

// STD includes
#include <functional>
#include <vector>
#include <string>

// Testing namespace
using namespace SHA_Sort;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int InvSortedArrayInt[] = {366, 212, 36, 15, 8, 2, 0, -2, -3};    // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                               // Random string

  typedef std::vector<int> Container_type;
  typedef Container_type::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;
  typedef std::greater_equal<Iterator_type::value_type> Greater_Comparator_type;
}
#endif /* DOXYGEN_SKIP */

// Basic Partition tests
TEST(TestPartition, Partitions)
{
  // Normal Run - Random Array
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
    Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_type pivot = sortedArray.begin() + 5;

    Partition<Iterator_type, Comparator_type>(sortedArray.begin(), pivot, sortedArray.end());

    int i = 0;
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end(); ++it, ++i)
      EXPECT_EQ(SortedArrayInt[i], *it);
  }

  // Begin and End inversed - Array should not be affected
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;

    Partition<Iterator_type, Comparator_type>(randomdArray.end(), pivot, randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  //String collection - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
  {
    std::string randomStr = RandomStr;
    std::string::iterator pivot = randomStr.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    Partition<std::string::iterator, std::less_equal<char>>(randomStr.begin(), pivot, randomStr.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (std::string::iterator it = randomStr.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (std::string::iterator it = pivot; it < randomStr.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }
}

// Extreme Pivot Partition tests
TEST(TestPartition, PartitionBoudaryPivots)
{
  // Pivot choose as begin
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin();
    const int pivotValue = *pivot;

    // Run partition
    Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Pivot choose as last element
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.end() - 1;
    const int pivotValue = *pivot;

    // Run partition
    Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Pivot choose as end - cannot process
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.end();

    // Run partition
    Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }
}

// Basic Partition tests - Greater element in the left partition
TEST(TestPartition, PartitionGreaterComparator)
{
  // Normal Run - Should result in: min[begin, pivot[ >= pivot >= max]pivot, end]
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    Partition<Iterator_type, Greater_Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are greater or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_LE(*pivot, *it);
    // All elements before the pivot are less or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_GE(*pivot, *it);
  }

  // Already InverseSortedArray - Array should not be affected
  {
    Container_type invSortedArray
      (InvSortedArrayInt, InvSortedArrayInt + sizeof(InvSortedArrayInt) / sizeof(int));
    Iterator_type pivot = invSortedArray.begin() + 5;

    Partition<Iterator_type, Greater_Comparator_type>(invSortedArray.begin(), pivot, invSortedArray.end());

    int i = 0;
    for (Iterator_type it = invSortedArray.begin(); it < invSortedArray.end(); ++it, ++i)
      EXPECT_EQ(invSortedArray[i], *it);
  }

  // String collection - Should result in: min[begin, pivot[ >= pivot >= max]pivot, end]
  {
    std::string randomStr = RandomStr;
    std::string::iterator pivot = randomStr.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    Partition<std::string::iterator, std::greater_equal<char>>(randomStr.begin(), pivot, randomStr.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are greater or equal
    for (std::string::iterator it = randomStr.begin(); it < pivot; ++it)
      EXPECT_LE(*pivot, *it);
    // All elements before the pivot are less or equal
    for (std::string::iterator it = pivot; it < randomStr.end(); ++it)
      EXPECT_GE(*pivot, *it);
  }
}
