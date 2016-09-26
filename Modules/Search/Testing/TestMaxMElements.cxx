#include <gtest/gtest.h>
#include <max_m_elements.hxx>

using namespace SHA_Search;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
}
#endif /* DOXYGEN_SKIP */

// Test MaxMElements
TEST(TestSearch, MaxMElements)
{
  // Should return the max value [5] for a unique element search
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 1);
    EXPECT_EQ(5, kMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 3);
    EXPECT_EQ(5, kMaxElements[0]);
    EXPECT_EQ(5, kMaxElements[1]);
    EXPECT_EQ(4, kMaxElements[2]);
  }

  // Should return the last elements on sorted vector
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (kSortedArray.begin(), kSortedArray.end(), 4);
    EXPECT_EQ(366, kMaxElements[0]);
    EXPECT_EQ(212, kMaxElements[1]);
    EXPECT_EQ(36, kMaxElements[2]);
    EXPECT_EQ(15, kMaxElements[3]);
  }

  // Should return empty vector on insufficient vector
  {
    const Container_Type oneElementCollection = Container_Type(1, 2);
    const Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (oneElementCollection.begin(), oneElementCollection.end(), 2);
    EXPECT_EQ(0, static_cast<int>(kMaxElements.size()));
  }

  // Should return empty vector when looking for less than 1 elements
  {
    const Container_Type oneElementCollection = Container_Type(1, 2);
    const Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (oneElementCollection.begin(), oneElementCollection.end(), 0);
    EXPECT_EQ(0, static_cast<int>(kMaxElements.size()));
  }
}

// Test MaxMElements to find the lowest values
TEST(TestSearch, MaxNElementsLowestValues)
{
  // Should return the min value [-18] for a unique element search
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::less_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 1);
    EXPECT_EQ(-18, kMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::less_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 3);
    EXPECT_EQ(-18, kMaxElements[0]);
    EXPECT_EQ(-5, kMaxElements[1]);
    EXPECT_EQ(2, kMaxElements[2]);
  }

  // Should return the first elements on sorted vector
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const Container_Type kMaxElements =
      MaxMElements<Container_Type, Const_Iterator_Type, std::less_equal<int>>
      (kSortedArray.begin(), kSortedArray.end(), 4);
    EXPECT_EQ(-3, kMaxElements[0]);
    EXPECT_EQ(-2, kMaxElements[1]);
    EXPECT_EQ(0, kMaxElements[2]);
    EXPECT_EQ(2, kMaxElements[3]);
  }
}
