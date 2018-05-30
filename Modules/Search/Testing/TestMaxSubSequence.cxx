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
#include <max_sub_sequence.hxx>

// STD includes
#include <functional>

// Testing namespace
using namespace HUC_Search;

#ifndef DOXYGEN_SKIP
namespace {
  // Simple sorted array of integers with negative values
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};
  // Simple random array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Test MaxSubSequence
TEST(TestSearch, MaxSubSequence)
{
  // Should return <5,9> (maximal sum of 17)
  {
  Container kMarketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const auto kMaxBeneficeIndexes = MaxSubSequence<IT>(kMarketPrices.begin(), kMarketPrices.end());
    EXPECT_EQ(5, kMaxBeneficeIndexes.first);
    EXPECT_EQ(9, kMaxBeneficeIndexes.second);
  }

  // Should return <FirstPositiveIdx, idxEnd> on sorted array
  {
    Container kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const auto kIndexes = MaxSubSequence<IT>(kSortedArray.begin(), kSortedArray.end());
    EXPECT_EQ(2, kIndexes.first);
    EXPECT_EQ(static_cast<int>(kSortedArray.size()) - 1, kIndexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    Container insufficientArray = Container(1, 2);
    const auto kIndexes = MaxSubSequence<IT>(insufficientArray.begin(), insufficientArray.end());
    EXPECT_EQ(-1, kIndexes.first);
    EXPECT_EQ(-1, kIndexes.second);
  }

  // Should return <0,1> on array containing only two positive elements
  {
    Container twoElementArray = Container(2, 2);
    const auto kIndexes = MaxSubSequence<IT>(twoElementArray.begin(), twoElementArray.end());
    EXPECT_EQ(0, kIndexes.first);
    EXPECT_EQ(1, kIndexes.second);
  }

  // Should return <0, idxEnd> on array containing the same positive value
  {
    const int kSize = 10;
    Container sameElementArray = Container(kSize, 2);
    const auto indexes = MaxSubSequence<IT>(sameElementArray.begin(), sameElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(kSize - 1, indexes.second);
  }
}
