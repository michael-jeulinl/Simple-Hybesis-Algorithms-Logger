#include <gtest/gtest.h>
#include <intersection.hxx>

// STD includes
#include <list>

using namespace SHA_Combinatory;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};     // Other  random array of integers with negative values
  const std::string OrderedStr = "acegmnoop";                        // Ordered string
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
}
#endif /* DOXYGEN_SKIP */

// Test intersections
TEST(TestIntersection, Intersections)
{
  // Null intersection on empty vectors - empty intersection expected
  {
    const Container_Type kEmptyCollection = Container_Type();
    Container_Type intersection = Intersection<Container_Type, Const_Iterator_Type>
      (kEmptyCollection.begin(), kEmptyCollection.end(), kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_EQ(0, intersection.size());
  }

  // Null intersection with one empty vector - empty intersection expected
  {
    const Container_Type kEmptyCollection = Container_Type();
    const Container_Type kCollection = Container_Type(10,1);
    Container_Type intersection = Intersection<Container_Type, Const_Iterator_Type>
      (kEmptyCollection.begin(), kEmptyCollection.end(), kCollection.begin(), kCollection.end());
    EXPECT_EQ(0, intersection.size());
  }

  // Basic run with same collection - intersection as the collection itself expected
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) /
      sizeof(Container_Type::value_type));
    Container_Type intersection = Intersection<Container_Type, Const_Iterator_Type>
      (kSortedArray.begin(), kSortedArray.end(), kSortedArray.begin(), kSortedArray.end());

    // The intersection of the same vector should return the same vector
    Const_Iterator_Type kIntersectIt = intersection.begin();
    for (Const_Iterator_Type it = kSortedArray.begin(); it != kSortedArray.end(); ++it, ++kIntersectIt)
      EXPECT_EQ(*it, *kIntersectIt);
  }

  // Basic run with same collection containing duplicates- intersection as the collection itself expected
  {
    const Container_Type kFirstRandom(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) /
      sizeof(Container_Type::value_type));
    const Container_Type kSecondRandom(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) /
      sizeof(Container_Type::value_type));

    Container_Type intersection = Intersection<Container_Type, Const_Iterator_Type>
      (kFirstRandom.begin(), kFirstRandom.end(), kSecondRandom.begin(), kSecondRandom.end());

    // The intersection with the copy vector should return the same vector
    Const_Iterator_Type kIntersectIt = intersection.begin();
    for (Const_Iterator_Type it = kFirstRandom.begin(); it != kFirstRandom.end(); ++it, ++kIntersectIt)
      EXPECT_EQ(*it, *kIntersectIt);
  }

  // Basic run with normal values
  {
    const Container_Type kFirstRandom
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Container_Type::value_type));
    const Container_Type kSecondRandom
      (RandomArrayInterInt, RandomArrayInterInt + sizeof(RandomArrayInterInt) /
      sizeof(Container_Type::value_type));

    Container_Type intersection = Intersection<Container_Type, Const_Iterator_Type>
      (kFirstRandom.begin(), kFirstRandom.end(), kSecondRandom.begin(), kSecondRandom.end());

    // Should return [-18, -5, 3, 5, 5] after sorting
    std::sort(intersection.begin(), intersection.end());
    EXPECT_EQ(-18, intersection[0]);
    EXPECT_EQ(-5, intersection[1]);
    EXPECT_EQ(3, intersection[2]);
    EXPECT_EQ(5, intersection[3]);
    EXPECT_EQ(5, intersection[4]);
  }

  // String run
  {
    std::string intersection = Intersection<std::string, std::string::const_iterator>
      (OrderedStr.begin(), OrderedStr.end(), RandomStr.begin(), RandomStr.end());

    // Should return ['a', 'c', 'e', 'g'] after sorting
    std::sort(intersection.begin(), intersection.end());
    EXPECT_EQ('a', intersection[0]);
    EXPECT_EQ('c', intersection[1]);
    EXPECT_EQ('e', intersection[2]);
    EXPECT_EQ('g', intersection[3]);
  }
}
