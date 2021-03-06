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
#ifndef MODULE_SEARCH_DATA_LOG_HXX
#define MODULE_SEARCH_DATA_LOG_HXX

// STD includes
#include <map>
#include <string>
#include <vector>

namespace DATA {
  static std::map<std::string, std::vector<int>> Integers
  {
    // Random ranged
    { "Int_Rand_5", { 0, 2, -2, -1, 1 } },
    { "Int_Rand_10", { 1, -4, 2, 3, -1, 4, 0 , -2, -5, -3 } },
    { "Int_Rand_20", { -5, 4, 7, -4, 9, 0, -10, -7, -2, 8, -3, 2, 3, 1, -8, -1, 10, -9, 6, -6 } },
    { "Int_Rand_50", { 18, -9, 2, 25, 16, -24, 6, -7, 11, -15, -21, -20, 3, 10, -12, 20, -18, -3, 1, -25, 5,
        -5, 14, 23, -10, -22, -14, -11, -4, 21, -2, 13, 19, -8, 15, 8, 12, -23, 17, -19, -6, 22, 4, 7, 9, -16,
        0, -17, -1, -13 }
    },
    { "Int_Rand_100", { -23, 36, 4, 13, -31, 32, 16, -43, 44, -13, -20, -46, -17, 49, -2, -36, -8, 47, -28,
        -48, 11, -27, 46, 29, -12, -47, 34, -41, -21, -3, -44, 26, -15, 14, 38, 17, -25, -40, 8, -32, -30,
        -38, 48, -22, -11, 7, 9, -49, -45, 21, -34, 27, 10, 2, 45, 22, -18, 30, 31, -33, -6, 5, 20, 6, -39,
        39, 37, 0, 43, -19, 35, 40, 1, -35, -10, 25, 50, -1, -16, -50, -37, -24, -9, -29, -4, 12, 19, -5, 23,
        41, 33, 42, 3, 24, 28, -14, 15, -7, 18, -26 }
    },

    // Few Uniques
    { "Int_Rand_Few_5", { -1, 1, 1, -1, 0 } },
    { "Int_Rand_Few_10", { -6, -10, 9, -1, 1, 1, -1, 0, 3, -3 } },
    { "Int_Rand_Few_20", { -10, -3, -8, 0, 10, -1, 0, -8, -2, -5, -1, -5, 6, 2, -6, -6, 2, 1, 0, -2 } },
    { "Int_Rand_Few_50", { 25, -21, -20, -10, -4, -21, 17, -6, 13, -2, -25, 2, -25, 14, 9, -9, -2, -6, 14,
        -21, 5, -10, -25, -12, 24, -2, -21, 13, 4, 0, -15, -6, 25, -22, -9, 1, -19, -8, -20, 25, -1, 4, 3,
        8, 16, -24, -18, 15, -17, 5 }
    },
    { "Int_Rand_Few_100", { 19, -11, -43, -22, -32, -25, -45, -39, 4, 34, -17, -31, -26, 1, 29, -9, 49, 26,
        -50, -45, 5, 38, 40, 19, 1, -8, -7, -34, 6, -35, 41, -35, 44, 6, 35, 15, -31, 19, 48, -31, 46, 9, -50,
        -11, -7, 8, -32, -46, -44, -36, -11, 41, -12, 37, -34, 0, 4, 35, 1, 12, 41, -47, 40, 18, 12, 22, -10,
        12, -39, -29, 42, -41, -31, -33, -39, -4, 13, 37, 29, -48, -36, -25, -32, 44, 35, -15, 48, 31, -49,
        -5, -11, 28, 29, 28, -23, 22, -16, -9, 31, -36 }
    },

    // Nearly sorted
    { "Int_Near_20", { -9, -10, -7, -8, -6, -4, -5, -2, -3, 0, -1, 1, 2, 4, 3, 6, 7, 8, 9, 10 } },
    { "Int_Near_50", { -25, -24, -22, -23, -21, -20, -19, -18, -17, -15, -16, -14, -13, -11, -12, -10, -9,
        -8, -7, -6, -4, -5, -2, -3, -1, 0, 1, 2, 3, 5, 4, 6, 7, 9, 8, 10, 12, 11, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 25, 23 }
    },
    { "Int_Near_100", { -50, -49, -47, -48, -46, -44, -45, -43, -41, -39, -40, -38, -37, -36, -34, -35,
        -33, -31, -32, -30, -28, -29, -25, -27, -26, -24, -22, -23, -21, -19, -20, -18, -17, -16, -15, -14,
        -13, -11, -12, -10, -9, -8, -6, -7, -4, -5, -2, -3, -1, 0, 2, 1, 3, 4, 5, 6, 7, 8, 9, 10, 12, 11, 13,
        14, 15, 16, 18, 17, 19, 20, 21, 23, 22, 24, 25, 27, 26, 28, 29, 30, 31, 32, 35, 33, 34, 36, 38, 37,
        39, 40, 41, 42, 43, 45, 44, 47, 46, 48, 49, 50 }
    }
  };

  // Index of searched keys
  static std::map<int, std::vector<int>> IndexSearch
  {
    // Random ranged
    { 5, { 2, 4 } },
    { 10, { 3, 5, 9 } },
    { 20, { 4, 10, 12 } },
    { 50, { 3, 25, 40 } },
    { 100, { 3, 25, 50, 75 } }
  };
}

#endif // MODULE_SEARCH_DATA_LOG_HXX
