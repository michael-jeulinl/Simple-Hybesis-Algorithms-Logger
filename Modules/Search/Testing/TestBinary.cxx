#include <gtest/gtest.h>
#include <binary.hxx>

// Testing namespace
using namespace SHA_Search;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};       // Simple sorted array of floats with negative values
  const std::string OrderedStr = "acegmnoop";                        // Ordered string

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
  typedef std::vector<double>::const_iterator Const_Double_Iterator_Type;

  template <typename T>
  struct EQUIVALENT
  {
    bool operator()(const T& a, const T& b) const
    { return std::abs(a - b) < std::numeric_limits<T>::epsilon(); }
  };

  template <typename T>
  struct EQUAL
  {
    bool operator()(const T& a, const T& b) const { return a == b; }
  };
}
#endif /* DOXYGEN_SKIP */

// Basic BinarySearchIterative tests on a sorted array of ints
TEST(TestSearch, BinarySearchBasics)
{
  const Container_Type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  // Empty array
  {
    Container_Type emptyArray = Container_Type();
    const int index =
      BinarySearch<Const_Iterator_Type, int, EQUAL<int>>(emptyArray.begin(), emptyArray.end(), 0);
    EXPECT_EQ(-1, index); // Should return -1 on empty array
  }

  // First element
  {
    const int index =
      BinarySearch<Const_Iterator_Type, int, EQUAL<int>>(sortedArray.begin(), sortedArray.end(), -3);
    EXPECT_EQ(0, index);
  }

  // Existing random value
  {
    const int index =
      BinarySearch<Const_Iterator_Type, int, EQUAL<int>>(sortedArray.begin(), sortedArray.end(), 8);
    EXPECT_EQ(4, index);
  }

  // Non-existing element
  {
    const int index =
      BinarySearch<Const_Iterator_Type, int, EQUAL<int>>(sortedArray.begin(), sortedArray.end(), 1);
    EXPECT_EQ(-1, index);
  }

  // String collection - Find letter
  {
    const int index = BinarySearch<std::string::const_iterator, char, EQUAL<char>>
      (OrderedStr.begin(), OrderedStr.end(), 'm');
    EXPECT_EQ(4, index);
  }
}

// Basic BinarySearchIterative tests on a sorted array of doubles
TEST(TestSearch, BinarySearchDoubles)
{
  const std::vector<double> sortedDoubleArray
    (SortedDoubleArray, SortedDoubleArray + sizeof(SortedDoubleArray) / sizeof(double));

  // First element
  {
    const int index = BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(-.3));
    EXPECT_EQ(0, index);
  }

  // Existing element
  {
    const int index = BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(0.12));
    EXPECT_EQ(2, index);
  }

  // Non Existing element
  {
    const int index = BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(8.1));
    EXPECT_EQ(-1, index);
  }

  // Value in the middle when identical values
  {
    const std::vector<double> identicalArray = std::vector<double>(10, 3.);
    const int index = BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (identicalArray.begin(), identicalArray.end(), static_cast<const double>(3.));
    EXPECT_EQ(5, index);
  }
}
