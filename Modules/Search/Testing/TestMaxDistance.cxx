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
#include <max_distance.hxx>

// STD includes
#include <functional>

// Testing namespace
using namespace huc::search;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Test SimpleStockMarketProblem on a simple market array
TEST(TestSearch, MaxDistances)
{
  // Should return <4,9> (largest benefice of 6)
  {
    Container marketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const auto maxBeneficeIndexes = MaxDistance<IT>(marketPrices.begin(), marketPrices.end());
    EXPECT_EQ(4, maxBeneficeIndexes.first);
    EXPECT_EQ(9, maxBeneficeIndexes.second);
  }

  // Should return <0, idxEnd> on sorted array
  {
    Container sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const auto indexes = MaxDistance<IT>(sortedArray.begin(), sortedArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(static_cast<int>(sortedArray.size()) - 1, indexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    Container insufficientArray = Container(1, 2);
    const auto indexes = MaxDistance<IT>(insufficientArray.begin(), insufficientArray.end());
    EXPECT_EQ(-1, indexes.first);
    EXPECT_EQ(-1, indexes.second);
  }

  // Should return <0,1> on array containing only two elements
  {
    Container twoElementArray = Container(2, 2);
    const auto indexes = MaxDistance<IT>(twoElementArray.begin(), twoElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // Should return <0,1> on array containing the same value
  {
    Container sameElementArray = Container(10, 2);
    const auto indexes = MaxDistance<IT>(sameElementArray.begin(), sameElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // String - should return <1,6> as 'a', 'z' are the most distanced letter
  {
    const auto indexes =
      MaxDistance<std::string::const_iterator, std::minus<char>>(RandomStr.begin(), RandomStr.end());
    EXPECT_EQ(1, indexes.first);
    EXPECT_EQ(6, indexes.second);
  }
}
