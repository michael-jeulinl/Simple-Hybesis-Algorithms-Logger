/*===========================================================================================================
 *
 * SHA-L - Simple Hybesis Algorithm Logger
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms-Logger/blob/master/LICENSE
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
#include "data.hxx"
#include <aggregate_in_place_log.hxx>

// Testing namespace
using namespace SHA_Logger;

// STD includes
#include <fstream>
#include <functional>

#ifndef DOXYGEN_SKIP
namespace {
  typedef std::vector<int> Container;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Test AggregateInPlace with different integer sequences
TEST(TestAggregateInPlaceLog, build)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::RotatedIntegers.begin(); it != SHA_DATA::RotatedIntegers.end(); ++it)
  {
    Container data(it->second);
    IT pivot = data.begin() + it->first;
    OFStream fileStream(std::string(
      "Int_Rand_" + std::to_string(it->second.size()) + "_" + std::to_string(it->first) + ".json"));

    VecStats tmpVar;
    AggregateInPlaceLog<IT>::Build(fileStream, OpGetAll, data.begin(), pivot, data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test AggregateInPlace with elements in first bigger than second
TEST(TestAggregateInPlaceLog, FirstBigger)
{
  Container sizes({10, 20, 50});
  Container pivots({4, 12, 25}); // Create rotation on those pivots

  auto pivotIdx = pivots.begin();
  for (auto size = sizes.begin(); size != sizes.end(); ++size, ++pivotIdx)
  {
    Container data;
    data.reserve(*size);
    // Create first half with biggest elements sorted
    for (auto i = 0; i < *pivotIdx; ++i)
      data.push_back((*size / 2) - *pivotIdx + i);
    // Create second half with smallest elements sorted
    for (auto i = *pivotIdx; i < *size; ++i)
      data.push_back(i - *pivotIdx - (*size / 2));

    OFStream fileStream(std::string("Int_First_" + std::to_string(*size) +
                                    "_" + std::to_string(*pivotIdx) + ".json"));
    VecStats tmpVar;
    IT pivot = data.begin() + *pivotIdx;
    AggregateInPlaceLog<IT>::Build(fileStream, OpGetAll, data.begin(), pivot, data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test AggregateInPlace with elements in second bigger than first
TEST(TestAggregateInPlaceLog, SecondBigger)
{
  Container sizes({10, 20, 50});
  Container pivots({4, 12, 30}); // Create rotation on those pivots

  auto pivotIdx = pivots.begin();
  for (auto size = sizes.begin(); size != sizes.end(); ++size, ++pivotIdx)
  {
    Container data;
    data.reserve(*size);
    // Create first half with biggest elements sorted
    for (auto i = 0; i < *pivotIdx; ++i)
      data.push_back(-(*size / 2) + i);
    // Create second half with smallest elements sorted
    for (auto i = *pivotIdx; i < *size; ++i)
      data.push_back((*size / 2) - *pivotIdx + i);

    OFStream fileStream(std::string("Int_Second_" + std::to_string(*size) +
                                    "_" + std::to_string(*pivotIdx) + ".json"));
    VecStats tmpVar;
    IT pivot = data.begin() + *pivotIdx;
    AggregateInPlaceLog<IT>::Build(fileStream, OpGetAll, data.begin(), pivot, data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test with random char sequences
TEST(TestAggregateInPlaceLog, RotatedChars)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::RotatedIntegers.begin(); it != SHA_DATA::RotatedIntegers.end(); ++it)
  {
    auto size = it->second.size();
    std::vector<char> data;
    data.reserve(size);
    for (auto i = 0; i < size; ++i)
      data.push_back(static_cast<char>(it->second[i] + (size / 2) + 65));
    std::vector<char>::iterator pivot = data.begin() + it->first;

    OFStream fileStream(std::string(
      "Char_Rand_" + std::to_string(it->second.size()) + "_" + std::to_string(it->first) + ".json"));

    VecStats tmpVar;
    AggregateInPlaceLog<std::vector<char>::iterator>::Build
      (fileStream, OpGetAll, data.begin(), pivot, data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}
