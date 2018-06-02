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
#include <max_m_elements.hxx>

using namespace huc::search;

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

// Test MaxMElements
TEST(TestSearch, MaxMElements)
{
  // Should return the max value [5] for a unique element search
  {
    Container kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container kMaxElements = MaxMElements<Container, IT>(kRandomElements.begin(), kRandomElements.end(), 1);
    EXPECT_EQ(5, kMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    Container kRandomElements (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container kMaxElements =MaxMElements<Container, IT>(kRandomElements.begin(), kRandomElements.end(), 3);
    EXPECT_EQ(5, kMaxElements[0]);
    EXPECT_EQ(5, kMaxElements[1]);
    EXPECT_EQ(4, kMaxElements[2]);
  }

  // Should return the last elements on sorted vector
  {
    Container kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Container kMaxElements = MaxMElements<Container, IT>(kSortedArray.begin(), kSortedArray.end(), 4);
    EXPECT_EQ(366, kMaxElements[0]);
    EXPECT_EQ(212, kMaxElements[1]);
    EXPECT_EQ(36, kMaxElements[2]);
    EXPECT_EQ(15, kMaxElements[3]);
  }

  // Should return empty vector on insufficient vector
  {
    Container uniqueEl = Container(1, 2);
    Container kMaxElements = MaxMElements<Container, IT>(uniqueEl.begin(), uniqueEl.end(), 2);
    EXPECT_EQ(0, static_cast<int>(kMaxElements.size()));
  }

  // Should return empty vector when looking for less than 1 elements
  {
    Container uniqueEl = Container(1, 2);
    Container kMaxElements = MaxMElements<Container, IT>(uniqueEl.begin(), uniqueEl.end(), 0);
    EXPECT_EQ(0, static_cast<int>(kMaxElements.size()));
  }
}

// Test MaxMElements to find the lowest values
TEST(TestSearch, MaxNElementsLowestValues)
{
  // Should return the min value [-18] for a unique element search
  {
    Container kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container kMaxElements =
      MaxMElements<Container, IT, std::less_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 1);
    EXPECT_EQ(-18, kMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    Container kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container kMaxElements = MaxMElements<Container, IT, std::less_equal<int>>
                                   (kRandomElements.begin(), kRandomElements.end(), 3);
    EXPECT_EQ(-18, kMaxElements[0]);
    EXPECT_EQ(-5, kMaxElements[1]);
    EXPECT_EQ(2, kMaxElements[2]);
  }

  // Should return the first elements on sorted vector
  {
    Container kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Container kMaxElements = MaxMElements<Container, IT, std::less_equal<int>>
                                   (kSortedArray.begin(), kSortedArray.end(), 4);
    EXPECT_EQ(-3, kMaxElements[0]);
    EXPECT_EQ(-2, kMaxElements[1]);
    EXPECT_EQ(0, kMaxElements[2]);
    EXPECT_EQ(2, kMaxElements[3]);
  }
}
