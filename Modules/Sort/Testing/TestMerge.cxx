#include <gtest/gtest.h>
#include <merge.hxx>

// STD includes
#include <functional>
#include <vector>
#include <string>

// Testing namespace
using namespace SHA_Sort;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int SortedArrayIntWithRot[] = {-3, 2, 7, 20, 0, 2, 8, 15, 36};    // Simple sorted array of integers with negative values
  const int SortedArrayIntPos[] = {0, 2, 8, 15, 36, 212, 366, 15478};     // Simple sorted array of integers with positive values only
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const int RandomArrayIntPos[] = {4520, 30, 500, 20, 3, 2, 3, 4, 5, 15}; // Simple random array of integers with positive values only
  const std::string RandomStr = "xacvgeze";                               // Random string
  const std::string StringWithPivot = "eknxasuw";                         // String with pivot at end =  begin() + 4 : Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]

  typedef std::vector<int> Container_type;
  typedef Container_type::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;
  typedef MergeWithBuffer<Container_type, Iterator_type> Aggregator_type;
  typedef MergeWithBuffer<std::string, std::string::iterator> AggregatorStr_type;
}
#endif /* DOXYGEN_SKIP */

// Basic MergeInPlace tests
TEST(TestMerge, MergeInPlaces)
{
  // Normal Run - All elements should be sorted in order
  {
    Container_type sortedArrayWithRot
      (SortedArrayIntWithRot, SortedArrayIntWithRot + sizeof(SortedArrayIntWithRot) / sizeof(int));
    MergeInPlace<Iterator_type>()
      (sortedArrayWithRot.begin(), sortedArrayWithRot.begin() + 4, sortedArrayWithRot.end());

    // All elements of the final array are sorted
    for (Iterator_type it = sortedArrayWithRot.begin(); it < sortedArrayWithRot.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    MergeInPlace<Iterator_type>()(sortedArrayPos.begin(), sortedArrayPos.begin() + 5, sortedArrayPos.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    MergeInPlace<Iterator_type>()(randomArrayPos.end(), randomArrayPos.begin() + 3, randomArrayPos.begin());

    int i = 0;
    for (Iterator_type it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    MergeInPlace<Iterator_type>()(emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    MergeInPlace<Iterator_type>()(uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container_type doubleValuesArray(1, 511);
    doubleValuesArray.push_back(66);

    MergeInPlace<Iterator_type>()
      (doubleValuesArray.begin(), doubleValuesArray.begin() + 1, doubleValuesArray.end());

    EXPECT_EQ(66, doubleValuesArray[0]);
    EXPECT_EQ(511, doubleValuesArray[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string stringToMerge = StringWithPivot;
    MergeInPlace<std::string::iterator>()
      (stringToMerge.begin(), stringToMerge.begin() + 4, stringToMerge.end());

    // All elements of the final array are sorted
    for (std::string::iterator it = stringToMerge.begin(); it < stringToMerge.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}


// Basic MergeWithBuffer tests
TEST(TestMerge, MergeWithBuffers)
{
  // Normal Run - All elements should be sorted in order
  {
    Container_type sortedArrayWithRot
      (SortedArrayIntWithRot, SortedArrayIntWithRot + sizeof(SortedArrayIntWithRot) / sizeof(int));
    MergeWithBuffer<Container_type, Iterator_type>()
      (sortedArrayWithRot.begin(), sortedArrayWithRot.begin() + 4, sortedArrayWithRot.end());

    // All elements of the final array are sorted
    for (Iterator_type it = sortedArrayWithRot.begin(); it < sortedArrayWithRot.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    MergeWithBuffer<Container_type, Iterator_type>()
      (sortedArrayPos.begin(), sortedArrayPos.begin() + 5, sortedArrayPos.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    MergeWithBuffer<Container_type, Iterator_type>()
      (randomArrayPos.end(), randomArrayPos.begin() + 3, randomArrayPos.begin());

    int i = 0;
    for (Iterator_type it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    MergeWithBuffer<Container_type, Iterator_type>()
      (emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    MergeWithBuffer<Container_type, Iterator_type>()
      (uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container_type doubleValuesArray(1, 511);
    doubleValuesArray.push_back(66);

    MergeWithBuffer<Container_type, Iterator_type>()
      (doubleValuesArray.begin(), doubleValuesArray.begin() + 1, doubleValuesArray.end());

    EXPECT_EQ(66, doubleValuesArray[0]);
    EXPECT_EQ(511, doubleValuesArray[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string stringToMerge = "eknxasuw"; // Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]
    MergeWithBuffer<std::string, std::string::iterator>()
      (stringToMerge.begin(), stringToMerge.begin() + 4, stringToMerge.end());

    // All elements of the final array are sorted
    for (std::string::iterator it = stringToMerge.begin(); it < stringToMerge.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Basic Merge-Sort tests - Uses the merge with buffer (computation optimized)
TEST(TestMerge, MergeSorts)
{
  // Normal Run - all elements should be sorter in order
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    MergeSort<Container_type, Iterator_type, Aggregator_type>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    MergeSort<Container_type, Iterator_type, Aggregator_type>(sortedArray.begin(), sortedArray.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    MergeSort<Container_type, Iterator_type, Aggregator_type>(randomdArray.end(), randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    MergeSort<Container_type, Iterator_type, Aggregator_type>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    MergeSort<Container_type, Iterator_type, Aggregator_type>
      (uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String collection - all elements should be sorter in order
  {
    std::string randomStr = RandomStr;
    MergeSort<std::string, std::string::iterator, AggregatorStr_type>
      (randomStr.begin(), randomStr.end());

    // All elements are sorted
    for (std::string::iterator it = randomStr.begin(); it < randomStr.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}
