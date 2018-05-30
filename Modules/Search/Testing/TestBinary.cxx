/*===========================================================================================================
 *
 * HUC - Hurna Core
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/Hurna/Hurna-Core/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#include <gtest/gtest.h>
#include <binary.hxx>

// Testing namespace
using namespace HUC_Search;

#ifndef DOXYGEN_SKIP
namespace {
  // Simple sorted array of integers with negative values
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};
  // Simple sorted array of floats with negative values
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};
  // Ordered string
  const std::string OrderedStr = "acegmnoop";

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
  typedef std::vector<double>::iterator IT_DL;

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
  Container sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  // Empty array
  {
    Container emptyArray = Container();
    const auto index = BinarySearch<IT, EQUAL<int>>(emptyArray.begin(), emptyArray.end(), 0);
    EXPECT_EQ(-1, index); // Should return -1 on empty array
  }

  // First element
  {
    const auto index = BinarySearch<IT, EQUAL<int>>(sortedArray.begin(), sortedArray.end(), -3);
    EXPECT_EQ(0, index);
  }

  // Existing random value
  {
    const auto index = BinarySearch<IT, EQUAL<int>>(sortedArray.begin(), sortedArray.end(), 8);
    EXPECT_EQ(4, index);
  }

  // Non-existing element
  {
    const auto index = BinarySearch<IT, EQUAL<int>>(sortedArray.begin(), sortedArray.end(), 1);
    EXPECT_EQ(-1, index);
  }

  // String collection - Find letter
  {
    const auto index = BinarySearch<std::string::const_iterator, EQUAL<char>>
      (OrderedStr.begin(), OrderedStr.end(), 'm');
    EXPECT_EQ(4, index);
  }
}

// Basic BinarySearchIterative tests on a sorted array of doubles
TEST(TestSearch, BinarySearchDoubles)
{
   std::vector<double> sortedDoubleArray
     (SortedDoubleArray, SortedDoubleArray + sizeof(SortedDoubleArray) / sizeof(double));

  // First element
  {
    const auto index = BinarySearch<IT_DL, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(-.3));
    EXPECT_EQ(0, index);
  }

  // Existing element
  {
    const auto index = BinarySearch<IT_DL, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(0.12));
    EXPECT_EQ(2, index);
  }

  // Non Existing element
  {
    const auto index = BinarySearch<IT_DL, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(8.1));
    EXPECT_EQ(-1, index);
  }

  // Value in the middle when identical values
  {
    std::vector<double> identicalArray = std::vector<double>(10, 3.);
    const auto index = BinarySearch<IT_DL, EQUIVALENT<double>>
      (identicalArray.begin(), identicalArray.end(), static_cast<const double>(3.));
    EXPECT_EQ(5, index);
  }
}
