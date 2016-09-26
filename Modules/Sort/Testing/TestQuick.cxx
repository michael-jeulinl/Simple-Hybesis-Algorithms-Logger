#include <gtest/gtest.h>
#include <quick.hxx>

// STD includes
#include <functional>
#include <vector>
#include <string>

// Testing namespace
using namespace SHA_Sort;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                               // Random string

  typedef std::vector<int> Container_type;
  typedef Container_type::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;
  typedef std::greater_equal<Iterator_type::value_type> Greater_Comparator_type;
}
#endif /* DOXYGEN_SKIP */

// Basic Quick-Sort tests
TEST(TestSort, QuickSorts)
{
  // Normal Run
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    QuickSort<Iterator_type, Comparator_type>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    QuickSort<Iterator_type, Comparator_type>(sortedArray.begin(), sortedArray.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    QuickSort<Iterator_type, Comparator_type>(randomdArray.end(), randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  // No error unitialized array
  {
    Container_type emptyArray;
    QuickSort<Iterator_type, Comparator_type>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    QuickSort<Iterator_type, Comparator_type>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String - String should be sorted as an array
  {
    std::string stringToSort = RandomStr;
    QuickSort<std::string::iterator, std::less_equal<char>>(stringToSort.begin(), stringToSort.end());
    EXPECT_EQ('a', stringToSort[0]);
    EXPECT_EQ('c', stringToSort[1]);
    EXPECT_EQ('e', stringToSort[2]);
    EXPECT_EQ('e', stringToSort[3]);
    EXPECT_EQ('g', stringToSort[4]);
    EXPECT_EQ('v', stringToSort[5]);
    EXPECT_EQ('x', stringToSort[6]);
    EXPECT_EQ('z', stringToSort[7]);
  }
}

// Basic Quick-Sort tests - Inverse Order
TEST(TestSort, QuickSortGreaterComparator)
{
  // Normal Run - Elements should be sorted in inverse order
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    QuickSort<Iterator_type, Greater_Comparator_type>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted in inverse order
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // Already sorted Array in inverse order - Array should not be affected
  {
    Container_type invSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    QuickSort<Iterator_type, Greater_Comparator_type>(invSortedArray.begin(), invSortedArray.end());

    // All elements are still sorted in inverse order
    for (Iterator_type it = invSortedArray.begin(); it < invSortedArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // String - String should be sorted in inverse order
  {
    std::string stringToSort = RandomStr;
    QuickSort<std::string::iterator, std::greater_equal<char>>(stringToSort.begin(), stringToSort.end());

    // All elements are sorted in inverse order
    for (std::string::iterator it = stringToSort.begin(); it < stringToSort.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }
}
