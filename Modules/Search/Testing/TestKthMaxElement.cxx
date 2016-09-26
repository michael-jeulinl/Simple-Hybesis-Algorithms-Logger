#include <gtest/gtest.h>
#include <kth_max_element.hxx>

// STD includes
#include <functional>

using namespace SHA_Search;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container_Type;
  typedef Container_Type::iterator Iterator_Type;
  typedef std::less_equal<Container_Type::value_type> Comparator_type;
  typedef std::greater_equal<Container_Type::value_type> Greater_Comparator_type;
}
#endif /* DOXYGEN_SKIP */

// Test kth smallest elements
TEST(TestSearch, KthSmallests)
{
  // Basic run on sorted array with unique element - Should the kth element
  {
    Container_Type ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_Type it = KthMaxElement<Iterator_Type, Comparator_type>
      (ksortedArray.begin(), ksortedArray.end(), 4);
    EXPECT_EQ(ksortedArray.begin() + 4, it);
  }

  // Basic run on random array - Should return 4
  {
    Container_Type krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_Type::value_type value = *KthMaxElement<Iterator_Type, Comparator_type>
      (krandomdArray.begin(), krandomdArray.end(), 7);
    EXPECT_EQ(4, value);
  }

  // Basic run with extreme values
  {
    Container_Type ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_Type it;

    // Empty sequence - Should return end on empty sequence
    it = KthMaxElement<Iterator_Type, Comparator_type>(ksortedArray.begin(), ksortedArray.begin(), 0);
    EXPECT_EQ(ksortedArray.begin(), it);

    // Unique element sequence - Should return the unique element
    it = KthMaxElement<Iterator_Type, Comparator_type>(ksortedArray.begin(), ksortedArray.begin() + 1, 0);
    EXPECT_EQ(ksortedArray.begin(), it);

    // Negative index - Should return end for out of scope search (k = -1)
    it = KthMaxElement<Iterator_Type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), -1);
    EXPECT_EQ(ksortedArray.end(), it);

    // k bigger than the size of the sequence - Should return end for out of scope search
    it =  KthMaxElement<Iterator_Type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), 100);
    EXPECT_EQ(ksortedArray.end(), it);
  }

  // String
  {
    std::string randomStr = RandomStr;
    const char secondSmallestLetter = *KthMaxElement
      <std::string::iterator, std::less_equal<char>>(randomStr.begin(), randomStr.end(), 1);
    EXPECT_EQ('c', secondSmallestLetter);
  }
}

// Test kth biggest element
TEST(TestSearch, KthBiggests)
{
  // Basic run on random array - Should return 5 (second biggest value)
  {
    Container_Type krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_Type::value_type value = *KthMaxElement<Iterator_Type, Greater_Comparator_type>
      (krandomdArray.begin(), krandomdArray.end(), 1);
    EXPECT_EQ(5, value);
  }
}
