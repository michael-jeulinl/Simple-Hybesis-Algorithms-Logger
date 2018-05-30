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
#include <raddix.hxx>

// Testing namespace
using namespace HUC_Sort;

#ifndef DOXYGEN_SKIP
namespace {
  // Simple sorted array of integers with positive values only
  const int SortedArrayIntPos[] = {0, 2, 8, 15, 36, 212, 366, 15478};
  // Simple random array of integers with positive values only
  const int RandomArrayIntPos[] = {4520, 30, 500, 20, 3, 2, 3, 4, 5, 15};

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Basic Quick-Sort tests
TEST(TestRaddix, RaddixSorts)
{
  // Normal Run - array should be sorted in order
  {
    Container randomdArrayPos(RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    RaddixSort<IT>(randomdArrayPos.begin(), randomdArrayPos.end());

    // All elements are sorted
    for (IT it = randomdArrayPos.begin(); it < randomdArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container sortedArrayPos(SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    RaddixSort<IT>(sortedArrayPos.begin(), sortedArrayPos.end());

    // All elements are still sorted
    for (IT it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container randomArrayPos(RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    RaddixSort<IT>(randomArrayPos.end(), randomArrayPos.begin());

    int i = 0;
    for (IT it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container emptyArray;
    RaddixSort<IT>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    RaddixSort<IT>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }
}
