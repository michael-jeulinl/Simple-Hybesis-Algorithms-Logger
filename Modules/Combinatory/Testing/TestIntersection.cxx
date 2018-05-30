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
#include <intersection.hxx>

// STD includes
#include <list>

using namespace HUC_Combinatory;

#ifndef DOXYGEN_SKIP
namespace {
  // Simple sorted array of integers with negative values
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};
  // Simple random array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};
  // Other  random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};
  // Ordered string
  const std::string OrderedStr = "acegmnoop";
  // Random string
  const std::string RandomStr = "xacvgeze";

  typedef std::vector<int> Container;
  typedef Container::value_type Value;
  typedef Container::const_iterator Const_IT;
}
#endif /* DOXYGEN_SKIP */

// Test intersections
TEST(TestIntersection, Intersections)
{
  // Null intersection on empty vectors - empty intersection expected
  {
    const Container kEmptyEl = Container();
    auto intersection = Intersection<Container, Const_IT>
      (kEmptyEl.begin(), kEmptyEl.end(), kEmptyEl.begin(), kEmptyEl.end());
    EXPECT_EQ(0, intersection.size());
  }

  // Null intersection with one empty vector - empty intersection expected
  {
    const Container kEmptyEl = Container();
    const Container kCollection = Container(10,1);
    Container intersection = Intersection<Container, Const_IT>
      (kEmptyEl.begin(), kEmptyEl.end(), kCollection.begin(), kCollection.end());
    EXPECT_EQ(0, intersection.size());
  }

  // Basic run with same collection - intersection as the collection itself expected
  {
    const Container kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(Value));
    Container intersection = Intersection<Container, Const_IT>
      (kSortedArray.begin(), kSortedArray.end(), kSortedArray.begin(), kSortedArray.end());

    // The intersection of the same vector should return the same vector
    Const_IT kIntersectIt = intersection.begin();
    for (Const_IT it = kSortedArray.begin(); it != kSortedArray.end(); ++it, ++kIntersectIt)
      EXPECT_EQ(*it, *kIntersectIt);
  }

  // Basic run with same collection containing duplicates- intersection as the collection itself expected
  {
    const Container kFirstRandom(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value));
    const Container kSecondRandom(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value));

    Container intersection = Intersection<Container, Const_IT>
      (kFirstRandom.begin(), kFirstRandom.end(), kSecondRandom.begin(), kSecondRandom.end());

    // The intersection with the copy vector should return the same vector
    Const_IT kIntersectIt = intersection.begin();
    for (Const_IT it = kFirstRandom.begin(); it != kFirstRandom.end(); ++it, ++kIntersectIt)
      EXPECT_EQ(*it, *kIntersectIt);
  }

  // Basic run with normal values
  {
    const Container kFirstRandom(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value));
    const Container kSecondRandom
      (RandomArrayInterInt, RandomArrayInterInt + sizeof(RandomArrayInterInt) / sizeof(Value));

    Container intersection = Intersection<Container, Const_IT>
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
