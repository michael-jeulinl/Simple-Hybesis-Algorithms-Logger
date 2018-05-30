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
#include <kth_order_statistic.hxx>

// STD includes
#include <functional>

using namespace HUC_Search;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
  typedef std::greater_equal<Container::value_type> GR_Compare;
}
#endif /* DOXYGEN_SKIP */

// Test kth smallest elements
TEST(TestSearch, KthOrderStatistic)
{
  {
    // Basic run on random array - Should return 4
    Container krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    EXPECT_EQ(4, *KthOrderStatistic<IT>(krandomdArray.begin(), krandomdArray.end(), 7));
  }

  Container ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  // Basic run on sorted array with unique element - Should the kth element
  EXPECT_EQ(ksortedArray.begin() + 4, KthOrderStatistic<IT>(ksortedArray.begin(), ksortedArray.end(), 4));

  // Empty sequence - Should return end on empty sequence
  EXPECT_EQ(ksortedArray.begin(), KthOrderStatistic<IT>(ksortedArray.begin(), ksortedArray.begin(), 0));

  // Unique element sequence - Should return the unique element
  EXPECT_EQ(ksortedArray.begin(), KthOrderStatistic<IT>(ksortedArray.begin(), ksortedArray.begin() + 1, 0));

  // Negative index - Should return end for out of scope search (k = 0)
  EXPECT_EQ(ksortedArray.begin(), KthOrderStatistic<IT>(ksortedArray.begin(), ksortedArray.end(), 0));

  // k bigger than the size of the sequence - Should return end for out of scope search
  EXPECT_EQ(ksortedArray.end(), KthOrderStatistic<IT>(ksortedArray.begin(), ksortedArray.end(), 100));

  // String
  {
    std::string randomStr = RandomStr;
    const char secondSmallestLetter = *KthOrderStatistic
      <std::string::iterator, std::less_equal<char>>(randomStr.begin(), randomStr.end(), 1);
    EXPECT_EQ('c', secondSmallestLetter);
  }
}

// Test kth min element
TEST(TestSearch, MinKthElement)
{
  // Basic run on random array - Should return 5 (second biggest value)
  Container krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
  IT::value_type value = *KthOrderStatistic<IT, GR_Compare>(krandomdArray.begin(), krandomdArray.end(), 1);
  EXPECT_EQ(5, value);
}
