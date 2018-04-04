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
#include <merge_log.hxx>

// STD includes
#include <fstream>
#include <functional>
#include <vector>

// Testing namespace
using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  typedef std::vector<int> Container;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */


// Test Merge with different integer sequences
TEST(TestMergeLog, build)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::Integers.begin(); it != SHA_DATA::Integers.end(); ++it)
  {
    Container data(it->second);
    OFStream fileStream(std::string(it->first + ".json"));

    VecStats tmpVar;
    MergeLog<IT>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Merge with reversed integer sequences
TEST(TestMergeLog, Reversed)
{
  Container sizes({10, 20, 50, 100});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    Container data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back((*size / 2) - i);

    OFStream fileStream(std::string("Int_Rev_" + std::to_string(*size) + ".json"));
    VecStats tmpVar;
    MergeLog<IT>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test with reversed char sequences
TEST(TestMergeLog, ReversedChars)
{
  Container sizes({10, 20, 50});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    std::vector<char> data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back(static_cast<char>(*size - i + 65));

    OFStream fileStream(std::string("Char_Rev_" + std::to_string(*size) + ".json"));
    VecStats tmpVar;
    MergeLog<std::vector<char>::iterator>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test with random char sequences
TEST(TestMergeLog, RandomChars)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::Integers.begin(); it != SHA_DATA::Integers.end(); ++it)
  {
    auto size = it->second.size();

    if (size > 50)
      continue;

    std::vector<char> data;
    data.reserve(size);
    for (auto i = 0; i < size; ++i)
      data.push_back(static_cast<char>(it->second[i] + (size / 2) + 65));

    OFStream fileStream(std::string("Char" + it->first.substr(3) + ".json"));

    VecStats tmpVar;
    MergeLog<std::vector<char>::iterator>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}
