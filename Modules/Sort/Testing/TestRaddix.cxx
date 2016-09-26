#include <gtest/gtest.h>
#include <raddix.hxx>

// Testing namespace
using namespace SHA_Sort;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayIntPos[] = {0, 2, 8, 15, 36, 212, 366, 15478};     // Simple sorted array of integers with positive values only
  const int RandomArrayIntPos[] = {4520, 30, 500, 20, 3, 2, 3, 4, 5, 15}; // Simple random array of integers with positive values only

  typedef std::vector<int> Container_type;
  typedef Container_type::iterator Iterator_type;
}
#endif /* DOXYGEN_SKIP */

// Basic Quick-Sort tests
TEST(TestRaddix, RaddixSorts)
{
  // Normal Run - array should be sorted in order
  {
    Container_type randomdArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    RaddixSort<Iterator_type>(randomdArrayPos.begin(), randomdArrayPos.end());

    // All elements are sorted
    for (Iterator_type it = randomdArrayPos.begin(); it < randomdArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    RaddixSort<Iterator_type>(sortedArrayPos.begin(), sortedArrayPos.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    RaddixSort<Iterator_type>(randomArrayPos.end(), randomArrayPos.begin());

    int i = 0;
    for (Iterator_type it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    RaddixSort<Iterator_type>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    RaddixSort<Iterator_type>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }
}
