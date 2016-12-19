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
#include <quick_log.hxx>

// STD includes
#include <fstream>
#include <functional>
#include <vector>

// Testing namespace
using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values

  typedef std::vector<int> Container_Type;
  typedef Container_Type::iterator Iterator_Type;
  typedef QuickLog<Iterator_Type, std::less_equal<int>> QuickLog_Type;
}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestQuickLog, build)
{
  Container_Type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
  OFStream_T fileStream("quick_output.json");
  QuickLog_Type::Build(fileStream, OpGetMin, randomdArray.begin(), randomdArray.end());
}
