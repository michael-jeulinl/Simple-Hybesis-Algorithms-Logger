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
#include <binary_log.hxx>

// Testing namespace
using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  // Simple sorted array of integers with negative values
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};

  template <typename T>
  struct EQUAL
  {
    bool operator()(const T& a, const T& b) const { return a == b; }
  };

  typedef std::vector<int> Container;
  typedef Container::const_iterator IT;
  typedef BinaryLog<IT, int, EQUAL<int>> BinaryLogT;
}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestBinaryLog, build)
{
  const Container sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  // Empty array
  {
    Container emptyArray = Container();
    BinaryLogT::Build(std::cout, OpGetAll, emptyArray.begin(), emptyArray.end(), 0);
  }

  BinaryLogT::Build(std::cout, OpGetMin, sortedArray.begin(), sortedArray.end(), -3);// First element
  BinaryLogT::Build(std::cout, OpGetMin, sortedArray.begin(), sortedArray.end(), 8); // Existing random value
}
