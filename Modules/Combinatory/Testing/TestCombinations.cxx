/*===========================================================================================================
 *
 * SHA - Simple Hybesis Algorithms
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms/blob/master/LICENSE
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
#include <combinations.hxx>

using namespace SHA_Combinatory;

#ifndef DOXYGEN_SKIP
namespace {
  const int SmallIntArray[] = {2, 1, 3};  // Small array containing 2, 1, 3 values

  typedef std::vector<int> Container;
  typedef Container::value_type Value;
  typedef Container::const_iterator Const_IT;
  typedef std::list<Container> List;
}
#endif /* DOXYGEN_SKIP */

// Test combinations
TEST(TestCombinations, Combinations)
{
  // Empty vector - no combinations
  {
    const Container kEmptyCollection = Container();
    List combinations = Combinations<Container, Const_IT>(kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_EQ(static_cast<size_t>(0), combinations.size());
  }

  // Inversed iterator - no combinations
  {
    const Container kUnicCollection = Container(1, 10);
    List combinations = Combinations<Container, Const_IT>(kUnicCollection.end(), kUnicCollection.begin());
    EXPECT_EQ(static_cast<size_t>(0), combinations.size());
  }

  // Unic element vector - Unique object returned as combinations
  {
    const Container kUnicCollection = Container(1, 10);
    List combinations = Combinations<Container, Const_IT>(kUnicCollection.begin(), kUnicCollection.end());
    EXPECT_EQ(static_cast<size_t>(1), combinations.size());
    EXPECT_EQ(static_cast<size_t>(1), combinations.begin()->size());
    EXPECT_EQ(10, *(combinations.begin()->begin()));
  }

  // Run with value 2, 1, 3
  {
    const Container kSmallArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value));
    List combinations = Combinations<Container, Const_IT>(kSmallArray.begin(), kSmallArray.end());
    EXPECT_EQ(static_cast<size_t>(7), combinations.size());

    int countEls = 0;
    for (List::const_iterator it = combinations.begin(); it != combinations.end(); ++it)
      countEls += static_cast<int>(it->size());
    EXPECT_EQ(12, countEls);
    //@TODO check sequence by sequence? (non ordered)
  }

  // String run
  {
    const std::string abcStr = "abc";
    std::list<std::string> combinations =
      Combinations<std::string, std::string::const_iterator>(abcStr.begin(), abcStr.end());
    EXPECT_EQ(static_cast<size_t>(7), combinations.size());

    int countEls = 0;
    for (std::list<std::string>::const_iterator it = combinations.begin(); it != combinations.end(); ++it)
      countEls += static_cast<int>(it->size());
    EXPECT_EQ(12, countEls);
    //@TODO check sequence by sequence? (non ordered)
  }
}
