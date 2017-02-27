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
#include <aggregate_in_place_log.hxx>

// Testing namespace
using namespace SHA_Logger;

// STD includes
#include <fstream>
#include <functional>
#include <vector>
#include <string>

#ifndef DOXYGEN_SKIP
namespace {
  // Sorted array with rotation
  //                 >------------------------------->||>--------------------->
  const int Data[] = {-8, -6, -4, -3, -1, 0, 2, 4, 6, -7, -5, -2, 1, 3, 5, 7, 8};

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestAggregateInPlaceLog, build)
{
  Container data(Data, Data + sizeof(Data) / sizeof(int));
  IT pivot = data.begin() + 9;

  OFStream fileStream("AggregateInPlace_output.json");

  // Run AggregateInPlace - Should result in fully sorted container
  AggregateInPlaceLog<IT>::Build(fileStream, OpGetAll, data.begin(), pivot, data.end());

  // All elements of the final array are sorted
  for (auto it = data.begin(); it < data.end() - 1; ++it)
    EXPECT_LE(*it, *(it + 1));
}
