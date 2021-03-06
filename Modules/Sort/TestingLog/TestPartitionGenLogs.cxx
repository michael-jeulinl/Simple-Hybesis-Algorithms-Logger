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
#include <partition_log.hxx>
#include "data.hxx"

// STD includes
#include <fstream>
#include <functional>

// Hurna Lib namespace
using namespace hul;
using namespace hul::sort;

#ifndef DOXYGEN_SKIP
namespace
{
  typedef Vector<int> Array;
  typedef Array::h_iterator IT;
  typedef Partition<IT> Sort;

  const std::string DIR = "partition";
}
#endif /* DOXYGEN_SKIP */

// Test Partition with different integer sequences
TEST(TestPartitionGenLogs, RandomIntegers)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::Integers.begin(); it != SHA_DATA::Integers.end(); ++it)
  {
    OFStream stream(DIR + "/" + it->first + ".json");
    auto logger = std::shared_ptr<Logger>(new Logger(stream));

    Array data(logger, it->second);
    auto pivot = std::find(data.h_begin(), data.h_end(), 0);
    pivot = Sort::Build(*logger.get(), data.h_begin(), pivot, data.h_end());

    // Element on the left side are smaller
    for (auto it = data.h_begin(); it < pivot; ++it)
      EXPECT_LE(*it, *pivot);

    // Element on the right side are bigger
    for (auto it = pivot; it < data.h_end(); ++it)
      EXPECT_GE(*it, *pivot);
  }
}

// Test Partition with reversed integer sequences
TEST(TestPartitionGenLogs, ReversedIntegers)
{
  std::vector<int> sizes({10, 20, 50, 100});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    OFStream stream(DIR + "/Int_Rev_" + ToString(*size) + ".json");
    auto logger = std::shared_ptr<Logger>(new Logger(stream));

    Array data(logger);
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back((*size / 2) - i);

    auto pivot = std::find(data.h_begin(), data.h_end(), 0);
    pivot = Sort::Build(*logger.get(), data.h_begin(), pivot, data.h_end());

    // Element on the left side are smaller
    for (auto it = data.h_begin(); it < pivot; ++it)
      EXPECT_LE(*it, *pivot);

    // Element on the right side are bigger
    for (auto it = pivot; it < data.h_end(); ++it)
      EXPECT_GE(*it, *pivot);
  }
}

// Test with reversed char sequences
TEST(TestPartitionGenLogs, ReversedChars)
{
  std::vector<int> sizes({10, 20, 50});

  for (auto size = sizes.begin(); size != sizes.end(); ++size)
  {
    OFStream stream(DIR + "/Char_Rev_" + ToString(*size) + ".json");
    auto logger = std::shared_ptr<Logger>(new Logger(stream));

    Vector<char> data(logger);
    data.reserve(*size);
    for (auto i = 0; i < *size; ++i)
      data.push_back(static_cast<char>(*size - i + 65));

    auto pivot = std::find(data.h_begin(), data.h_end(), static_cast<char>((*size / 2) + 65));
    pivot = Partition<Vector<char>::h_iterator>::Build(*logger.get(), data.h_begin(), pivot, data.h_end());

    // Element on the left side are smaller
    for (auto it = data.h_begin(); it < pivot; ++it)
      EXPECT_LE(*it, *pivot);

    // Element on the right side are bigger
    for (auto it = pivot; it < data.h_end(); ++it)
      EXPECT_GE(*it, *pivot);
  }
}

// Test with random char sequences
TEST(TestPartitionGenLogs, RandomChars)
{
  // Generate log for all Random integers
  for (auto it = SHA_DATA::Integers.begin(); it != SHA_DATA::Integers.end(); ++it)
  {
    auto size = it->second.size();
    if (size > 50)
      continue;

    OFStream stream(DIR + "/Char" + it->first.substr(3) + ".json");
    auto logger = std::shared_ptr<Logger>(new Logger(stream));

    Vector<char> data(logger);
    data.reserve(size);
    for (auto i = 0; i < size; ++i)
      data.push_back(static_cast<char>(it->second[i] + (size / 2) + 65));

    auto pivot = std::find(data.h_begin(), data.h_end(), static_cast<char>((size / 2) + 65));
    pivot = Partition<Vector<char>::h_iterator>::Build(*logger.get(), data.h_begin(), pivot, data.h_end());

    // Element on the left side are smaller
    for (auto it = data.h_begin(); it < pivot; ++it)
      EXPECT_LE(*it, *pivot);

    // Element on the right side are bigger
    for (auto it = pivot; it < data.h_end(); ++it)
      EXPECT_GE(*it, *pivot);
  }
}
