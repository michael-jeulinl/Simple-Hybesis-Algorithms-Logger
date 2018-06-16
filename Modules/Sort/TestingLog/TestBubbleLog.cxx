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
#include <bubble_log.hxx>

// Data @todo use the one centralized from core (?!)
#include "data.hxx"

// STD includes
#include <fstream>
#include <functional>
#include <vector>

// Testing namespace
using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  //typedef std::vector<int> Container;
  //typedef Container::iterator IT;

  typedef Vector<int> Vec;
  typedef Vec::h_iterator IT;
  typedef Bubble<IT> Sort;
}
#endif /* DOXYGEN_SKIP */

/*TEST(TestBubbleLog, build)
{
  Container data({ 1, -4, 2, 3, -1, 4, 0 , -2, -5, -3 });
  OFStream fileStream("bubble_log_origin.json");

  VecStats tmpVar;
  BubbleLog<IT>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

  // All elements of the final array are sorted
  for (auto it = data.begin(); it < data.end() - 1; ++it)
    EXPECT_LE(*it, *(it + 1));
}
*/
TEST(TestBubbleLog, buildFacto)
{
  //OFStream fileStream("bubble_log.json");
  // File Stream
  OFStream fileStream("bubble_log.json");
  auto logger = std::shared_ptr<Logger>(new Logger(fileStream));

  // Run
  //Vec data(logger, { 1 });
  Vec data(logger, { 1, -4, 2, 3, -1, 4, 0 , -2, -5, -3 });
  Sort::Build(*logger.get(), data.h_begin(), data.h_end());


  // All elements of the final array are sorted
  for (auto it = data.begin(); it < data.end() - 1; ++it)
    EXPECT_LE(*it, *(it + 1));
}





// Test Bubble with different integer sequences
/*TEST(TestBubbleLog, build)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::Integers.begin(); it != SHA_DATA::Integers.end(); ++it)
  {
    Container data(it->second);
    OFStream fileStream(std::string(it->first + ".json"));

    VecStats tmpVar;
    BubbleLog<IT>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Bubble with reversed integer sequences
/*TEST(TestBubbleLog, Reversed)
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
    BubbleLog<IT>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test with reversed char sequences
TEST(TestBubbleLog, ReversedChars)
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
    BubbleLog<std::vector<char>::iterator>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test with random char sequences
TEST(TestBubbleLog, RandomChars)
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
    BubbleLog<std::vector<char>::iterator>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}*/
