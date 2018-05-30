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
#include <permutations.hxx>

using namespace HUC_Combinatory;

#ifndef DOXYGEN_SKIP
namespace {
  const int SmallIntArray[] = {2, 1, 3};  // Small array containing 2, 1, 3 values

  typedef std::vector<int> Container;
  typedef Container::const_iterator Const_IT;
}
#endif /* DOXYGEN_SKIP */

// Test permutations
TEST(TestCore, Permutations)
{
  // Empty vector - no permutations
  {
    const Container kEmptyCollection = Container();
    auto permutations = Permutations<Container, Const_IT>(kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_EQ(0, permutations.size());
  }

  // Inversed iterator - no permutations
  {
    const Container kUnicCollection = Container(1,10);
    auto permutations = Permutations<Container, Const_IT>(kUnicCollection.end(), kUnicCollection.begin());
    EXPECT_EQ(0, permutations.size());
  }

  // Unic element vector - Unique object returned as permutation
  {
    const Container kUnicCollection = Container(1,10);
    auto permutations = Permutations<Container, Const_IT>(kUnicCollection.begin(), kUnicCollection.end());
    EXPECT_EQ(1, permutations.size());
    EXPECT_EQ(1, permutations.begin()->size());
    EXPECT_EQ(10, *(permutations.begin()->begin()));
  }

  // Same elements vector - n! list of n equivalent elements
  {
    const Container kSameElCollection = Container(3,10);
    auto permutations = Permutations<Container, Const_IT>(kSameElCollection.begin(), kSameElCollection.end());
    EXPECT_EQ(6, permutations.size());
    for (auto it = permutations.begin(); it != permutations.end(); ++it)
    {
      EXPECT_EQ(3, it->size());
      for (Container::const_iterator itEl = it->begin(); itEl != it->end(); ++itEl)
        EXPECT_EQ(10, *itEl);
    }
  }

  // Run with value 2, 1, 3
  {
    const Container kSmallArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) /
      sizeof(Container::value_type));
    auto permutations = Permutations<Container, Const_IT>(kSmallArray.begin(), kSmallArray.end());
    EXPECT_EQ(6, permutations.size());
    for (auto it = permutations.begin(); it != permutations.end(); ++it)
      EXPECT_EQ(3, it->size());
    //@TODO check sequence by sequence? (non ordered)
  }

  // String run
  {
    const std::string abcStr = "abc";
    std::list<std::string> permutations =
      Permutations<std::string, std::string::const_iterator>(abcStr.begin(), abcStr.end());
    EXPECT_EQ(6, permutations.size());
    for (std::list<std::string>::const_iterator it = permutations.begin(); it != permutations.end(); ++it)
      EXPECT_EQ(3, it->size());
    //@TODO check sequence by sequence? (non ordered)
  }
}
