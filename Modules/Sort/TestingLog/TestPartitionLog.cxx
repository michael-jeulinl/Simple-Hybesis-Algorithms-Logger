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
using namespace SHA_Sort;

// STD includes
#include <fstream>
#include <functional>
#include <vector>
#include <string>

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int InvSortedArrayInt[] = {366, 212, 36, 15, 8, 2, 0, -2, -3};    // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                               // Random string

  typedef std::vector<int> Container_type;
  typedef Container_type::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;
  typedef std::greater_equal<Iterator_type::value_type> Greater_Comparator_type;
}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestPartitionLog, build)
{
  Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
  Iterator_type pivot = randomdArray.begin() + 5;
  const int pivotValue = *pivot;

  OFStream_T fileStream("partition_output.json");

  // Run partition - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
  PartitionLog<Iterator_type, Comparator_type>::Build
    (fileStream, OpGetAll, randomdArray.begin(), pivot, randomdArray.end());
}
