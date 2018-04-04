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
#include <quick_log.hxx>

// STD includes
#include <fstream>

// Testing namespace
using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  typedef std::vector<int> Container;
  typedef Container::iterator IT;

  typedef QuickLog<IT, std::less_equal<typename std::iterator_traits<IT>::value_type>, PickRandom<IT>>
    QuickLogPRand;
  typedef QuickLog<IT, std::less_equal<typename std::iterator_traits<IT>::value_type>, PickFirst<IT>>
    QuickLogPFirst;
  typedef QuickLog<IT, std::less_equal<typename std::iterator_traits<IT>::value_type>, PickLast<IT>>
    QuickLogPLast;
  typedef QuickLog<IT, std::less_equal<typename std::iterator_traits<IT>::value_type>, PickMiddle<IT>>
    QuickLogPMiddle;
  typedef QuickLog<IT, std::less_equal<typename std::iterator_traits<IT>::value_type>, PickThreeMedian<IT>>
    QuickLogPTMed;
}
#endif /* DOXYGEN_SKIP */

// Test Quick with different integer sequences
TEST(TestQuickLog, buildPRand)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::Integers.begin(); it != SHA_DATA::Integers.end(); ++it)
  {
    Container data(it->second);
    OFStream fileStream(std::string(it->first + "_prand.json"));

    VecStats tmpVar;
    QuickLogPRand::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick with reversed integer sequences
TEST(TestQuickLog, ReversedPRand)
{
  Container sizes({10, 20, 50, 100});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    Container data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back((*size / 2) - i);

    OFStream fileStream(std::string("Int_Rev_" + std::to_string(*size) + "_prand.json"));
    VecStats tmpVar;
    QuickLogPRand::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick using first picking strategy with different integer sequences
TEST(TestQuickLog, buildPFirst)
{
  std::vector<std::string> dataStr = {"Int_Rand_20", "Int_Rand_50", "Int_Rand_100"};

  // Generate log for all Random integers
  for (auto dataIt = dataStr.begin(); dataIt != dataStr.end(); ++dataIt)
  {
    auto it = SHA_DATA::Integers.find(*dataIt);

    Container data(it->second);
    OFStream fileStream(std::string(it->first + "_pfirst.json"));

    VecStats tmpVar;
    QuickLogPFirst::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick with reversed integer sequences
TEST(TestQuickLog, ReversedPFirst)
{
  Container sizes({20, 50, 100});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    Container data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back((*size / 2) - i);

    OFStream fileStream(std::string("Int_Rev_" + std::to_string(*size) + "_pfirst.json"));
    VecStats tmpVar;
    QuickLogPFirst::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick using last picking strategy with different integer sequences
TEST(TestQuickLog, buildPLast)
{
  std::vector<std::string> dataStr = {"Int_Rand_20", "Int_Rand_50", "Int_Rand_100"};

  // Generate log for all Random integers
  for (auto dataIt = dataStr.begin(); dataIt != dataStr.end(); ++dataIt)
  {
    auto it = SHA_DATA::Integers.find(*dataIt);

    Container data(it->second);
    OFStream fileStream(std::string(it->first + "_plast.json"));

    VecStats tmpVar;
    QuickLogPLast::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick using last picking strategy with reversed integer sequences
TEST(TestQuickLog, ReversedPLast)
{
  Container sizes({20, 50, 100});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    Container data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back((*size / 2) - i);

    OFStream fileStream(std::string("Int_Rev_" + std::to_string(*size) + "_plast.json"));
    VecStats tmpVar;
    QuickLogPLast::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick using middle picking strategy with different integer sequences
TEST(TestQuickLog, buildPMiddle)
{
  std::vector<std::string> dataStr = {"Int_Rand_20", "Int_Rand_50", "Int_Rand_100"};

  // Generate log for all Random integers
  for (auto dataIt = dataStr.begin(); dataIt != dataStr.end(); ++dataIt)
  {
    auto it = SHA_DATA::Integers.find(*dataIt);

    Container data(it->second);
    OFStream fileStream(std::string(it->first + "_pmiddle.json"));

    VecStats tmpVar;
    QuickLogPMiddle::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick using middle picking strategy with reversed integer sequences
TEST(TestQuickLog, ReversedPMiddle)
{
  Container sizes({20, 50, 100});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    Container data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back((*size / 2) - i);

    OFStream fileStream(std::string("Int_Rev_" + std::to_string(*size) + "_pmiddle.json"));
    VecStats tmpVar;
    QuickLogPMiddle::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick using three median picking strategy with different integer sequences
TEST(TestQuickLog, buildTmed)
{
  std::vector<std::string> dataStr = {"Int_Rand_20", "Int_Rand_50", "Int_Rand_100"};

  // Generate log for all Random integers
  for (auto dataIt = dataStr.begin(); dataIt != dataStr.end(); ++dataIt)
  {
    auto it = SHA_DATA::Integers.find(*dataIt);

    Container data(it->second);
    OFStream fileStream(std::string(it->first + "_ptmed.json"));

    VecStats tmpVar;
    QuickLogPTMed::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test Quick using three median picking strategy with reversed integer sequences
TEST(TestQuickLog, ReversedPTMed)
{
  Container sizes({20, 50, 100});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    Container data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back((*size / 2) - i);

    OFStream fileStream(std::string("Int_Rev_" + std::to_string(*size) + "_ptmed.json"));
    VecStats tmpVar;
    QuickLogPTMed::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test with reversed char sequences
TEST(TestQuickLog, ReversedChars)
{
  Container sizes({10, 20, 50});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    std::vector<char> data;
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back(static_cast<char>(*size - i + 65));

    OFStream fileStream(std::string("Char_Rev_" + std::to_string(*size) + "_prand.json"));
    VecStats tmpVar;
    QuickLog<std::vector<char>::iterator>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Test with random char sequences
TEST(TestQuickLog, RandomChars)
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

    OFStream fileStream(std::string("Char" + it->first.substr(3) + "_prand.json"));

    VecStats tmpVar;
    QuickLog<std::vector<char>::iterator>::Build(fileStream, OpGetAll, data.begin(), data.end(), tmpVar);

    // All elements of the final array are sorted
    for (auto it = data.begin(); it < data.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}
