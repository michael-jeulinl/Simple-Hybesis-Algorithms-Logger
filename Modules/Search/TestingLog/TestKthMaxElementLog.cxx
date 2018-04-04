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
#include <kth_max_element_log.hxx>

// STD includes
#include <fstream>

// Testing namespace
using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  std::map<std::string, std::vector<int>> Integers
  {
    // Random ranged
    { "Int_Rand_5", { 806, 1120, -737, -975, 675 } },
    { "Int_Rand_10", { 1424, 982, 109, 1409, 383, 1363, 384, 1493, 161, 1296 } },
    { "Int_Rand_20", { 1048, 74, 1467, 394, -417, 959, 533, -713, -159, 1411, 88, 996, 629, -1495, 1128, -637,
        784, -25, 895, -1333 } },
    { "Int_Rand_50", { 530, -568, -468, -1449, -624, -188, 643, -651, -409, -725, 275, -1016, -364, -825,
        277, 937, -1108, 1348, -951, -271, 485, 1287, -494, 1127, 178, -973, 1244, 197, 234, -659, -770, 1190,
        1010, 1161, -680, 1294, -1484, 987, 696, 63, -127, 1450, -779, -1466, 1374, 346, 1131, 918, 490, 460 }
    },
    { "Int_Rand_100", { 258, 23, 373, 87, 439, 381, 116, 412, 491, 242, 200, -46, -386, 271, 148, 495, 382,
        -106, 365, 166, -130, 219, 335, 98, 424, 73, 312, 69, 406, 11, 204, -86, 468, 129, 283, 160, 236, 402,
        -291, 109, 297, -65, 140, -195, -154, 296, 493, 187, 372, 127, -198, 435, -356, 33, 99, 292,
        -206, 426, 421, -430, 246, -183, -77, -16, -168, -257, 362, 443, -285, -448, -230, -275, -364,
        -225, 248, 437, -300, 186, -395, -132, 124, 169, -62, 205, -380, -67, 35, -281,
        -44, 57, 313, -390, 331, -50, 150, -272, -102, -21, -384, -72 }
    },

    // Few Uniques
    { "Int_Rand_Few_5", { 1319, 169, -118, 169, 319 } },
    { "Int_Rand_Few_10", { 1240, 374, 162, -1133, 681, -183, 1229, 374, 192, -1133 } },
    { "Int_Rand_Few_20", { 1048, 74, 1467, 74, -417, 959, 533, -713, -159, 88, 88, 996, 629, -1495, 629, -637,
        784, -25, 895, -1333 } },
    { "Int_Rand_Few_50", { 530, -568, -468, 277, 624, -188, -490, 651, 409, 696, -275, -1016, 364, 825, 277,
        937, 1108, 1348, -951, 271, -485, 1287, -494, -1127, -178, 973, 1244, -197, -234, 659, -770, 1190,
        1010, 1161, -191, -1294, -1484, 696, 696, -63, -127, 1450, 779, -1466, 1374, -346, 696, 918, -490,
        460 }
    },
    { "Int_Rand_Few_100", { 58, 123, 7, 152, 60, 75, 75, 70, 23, 190, 111, 44, -133, 18,
        18, 61, 191, 179, 166, -157, -173, 73, 1, -21, 101, 37, 111, -195, -14, 83, 39, 10,
        20, 39, -144, 161, 161, -114, 166, 175, 196, 128, 114, 22, 117, -195, 109, 154, 88,
        193, 130, 5, 76, 183, -197, -99, 138, 73, 140, 102, 22, -99, 84, 176, 179, 183, -99,
        188, 132, 125, -3, -3, 101, 135, 9, 59, 94, 103, 58, 54, 51, 139, 72, -20,
        172, 7, 151, 86, 107, 65, -3, -61, 39, 145, 110, 105, -20, 120, 5, 18 }
    },
  };

  std::map<int, std::vector<int>> IndexSearch
  {
    // Random ranged
    { 5, { 2, 4 } },
    { 10, { 3, 5, 9 } },
    { 20, { 4, 10, 12 } },
    { 50, { 3, 25, 40 } },
    { 100, { 3, 25, 50, 75 } }
  };

  typedef std::vector<int> Container;
  typedef std::vector<int>::iterator IT;
}
#endif /* DOXYGEN_SKIP */

// Test KthElement with different integer sequences
TEST(TestKthElementLog, build)
{
  // Generate log for all Random integers
  for (auto it = Integers.begin(); it != Integers.end(); ++it)
  {
    auto rangeIt = IndexSearch.find(it->second.size());
    if (rangeIt == IndexSearch.end())
      continue;

    for (auto idxIt = rangeIt->second.begin(); idxIt != rangeIt->second.end(); ++idxIt)
    {
      Container data(it->second);
      OFStream fileStream(std::string(it->first + "_" + std::to_string(*idxIt) + ".json"));

      VecStats tmpVar;
      KthElementLog<IT>::Build(fileStream, OpGetAll, data.begin(), data.end(), *idxIt, tmpVar);
    }
  }
}

