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

// Testing namespace
using namespace SHA_Logger;

// STD includes
#include <fstream>
#include <functional>
#include <vector>
#include <string>

#ifndef DOXYGEN_SKIP
namespace {
  // Simple random array of integers with negative values
//  const int RandomArrayInt[] = {40, 80, -10, 50, 1, 65, -80, 26, -20, -35, 35, 35, -70, 100, -50};
  const int RandomArrayInt[] = {-1, 6, -4, 5, 8, 4, -5, -6, 7, 0, 2, -2, -3, 3, 3, -7, -8, 1};

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestPartitionLog, build)
{
  Container randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
  IT pivot = randomdArray.begin() + std::distance(randomdArray.begin(), randomdArray.end()) / 2;

  OFStream fileStream("partition_output.json");

  // Run partition - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
  PartitionLog<IT>::Build(fileStream, OpGetAll, randomdArray.begin(), pivot, randomdArray.end());
}
