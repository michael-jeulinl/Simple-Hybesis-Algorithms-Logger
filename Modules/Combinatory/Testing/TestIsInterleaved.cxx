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
#include <is_interleaved.hxx>

using namespace huc::combinatory;

#ifndef DOXYGEN_SKIP
namespace {
  const int kSequenceAInt[] = {4, 3, 5};              // Simple random array of integers with negative values
  const int kSequenceBInt[] = {-2, 3, 5, 4};          // Simple random array of integers with negative values
  const int kSequenceCInt[] = {5, -2, 4, 3, 4, 5, 3}; // Interleave of Sequence A and Sequence B
  const int kSequenceDInt[] = {-2, 3};                // Simple random array of integers with negative values
  const std::string kSequenceAStr = "acegm";          // Random string
  const std::string kSequenceBStr = "xacvgeze";       // Random string
  const std::string kSequenceCStr = "axaemgccvgeze";  // Interleave of Sequence A and Sequence B

  typedef std::vector<int> Container;
  typedef Container::value_type Value;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Test interleaves
TEST(TestIsInterleaved, Interleaved)
{
  // Null interleave on empty sequences - empty interleave expected
  {
    Container empty = Container();
    EXPECT_TRUE(IsInterleaved<IT>
      (empty.begin(), empty.end(), empty.begin(), empty.end(), empty.begin(), empty.end()));
  }

  // interleave with one empty sequence (first) - same sequence expected as interleave
  {
    Container sequenceA(kSequenceAInt, kSequenceAInt + sizeof(kSequenceAInt) / sizeof(Value));
    EXPECT_TRUE(IsInterleaved<IT>(sequenceA.begin(), sequenceA.end(), sequenceA.end(),
      sequenceA.end(), sequenceA.begin(), sequenceA.end()));
  }

  // interleave with one empty sequence (second) - same sequence expected as interleave
  {
    Container sequenceA(kSequenceAInt, kSequenceAInt + sizeof(kSequenceAInt) / sizeof(Value));
    EXPECT_TRUE(IsInterleaved<IT>(sequenceA.end(), sequenceA.end(), sequenceA.begin(),
      sequenceA.end(), sequenceA.begin(), sequenceA.end()));
  }

  // interleave with same sequence - same sequence cannot be interleave of both
  {
    Container sequenceA(kSequenceAInt, kSequenceAInt + sizeof(kSequenceAInt) / sizeof(Value));
    EXPECT_FALSE(IsInterleaved<IT>(sequenceA.begin(), sequenceA.end(), sequenceA.begin(),
      sequenceA.end(), sequenceA.begin(), sequenceA.end()));
  }

  // Normal run with interleave
  {
    Container sequenceA(kSequenceAInt, kSequenceAInt + sizeof(kSequenceAInt) / sizeof(Value));
    Container sequenceB(kSequenceBInt, kSequenceBInt + sizeof(kSequenceBInt) / sizeof(Value));
    Container sequenceC(kSequenceCInt, kSequenceCInt + sizeof(kSequenceCInt) / sizeof(Value));
    EXPECT_TRUE(IsInterleaved<IT>(sequenceA.begin(), sequenceA.end(), sequenceB.begin(),
      sequenceB.end(), sequenceC.begin(), sequenceC.end()));
  }

  // Normal run with interleave on strings
  {
    std::string aStr = kSequenceAStr;
    std::string bStr = kSequenceBStr;
    std::string cStr = kSequenceCStr;
    EXPECT_TRUE(IsInterleaved<std::string::iterator>(aStr.begin(), aStr.end(), bStr.begin(),
      bStr.end(), cStr.begin(), cStr.end()));
  }

  // Run with extra letter on FullStr
  {
    std::string aStr = kSequenceAStr;
    std::string bStr = kSequenceBStr;
    std::string cStr = kSequenceCStr + 'a';
    EXPECT_FALSE(IsInterleaved<std::string::iterator>(aStr.begin(), aStr.end(), bStr.begin(),
      bStr.end(), cStr.begin(), cStr.end()));
  }
}
