/*===========================================================================================================
 *
 * HUC - Hurna Core
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/Hurna/Hurna-Core/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#ifndef MODULE_COMBINATORY_IS_INTERLEAVED_HXX
#define MODULE_COMBINATORY_IS_INTERLEAVED_HXX

// STD includes
#include <map>

namespace HUC_Combinatory
{
  /// IsInterleaved - Return whether or not if a sequence is the interleave of the two others.
  ///
  /// @complexity O(N + M + K).
  ///
  /// @tparam IT type using to go through the collection.
  ///
  /// @param beginFirst,endFirst,beginSecond,endSecond,beginFull,endFull - iterators to the initial and
  /// final positions of the sequences. The range used is [first,last), which contains all the elements
  /// between first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return true if the last sequence is the interleave of the two others, false otherwise.
  template <typename IT>
  bool IsInterleaved(const IT& beginFirst, const IT& endFirst,
                     const IT& beginSecond, const IT& endSecond,
                     const IT& beginFull, const IT& endFull)
  {
    // Lambda that count each element occurence within the map "count"
    std::map<typename std::iterator_traits<IT>::value_type, int> count;
    auto lCountElement = [&count](typename std::iterator_traits<IT>::value_type(val))
    {
      auto countIt = count.find(val);
      if (countIt != count.end())
        ++countIt->second;
      else
        count.emplace(val, 1);
    };

    // Count both sequences
    std::for_each(beginFirst, endFirst, lCountElement);
    std::for_each(beginSecond, endSecond, lCountElement);

    // Decrease the count of each element given the full sequence
    for (auto it = beginFull; it != endFull; ++it)
    {
      auto countIt = count.find(*it);
      if (countIt == count.end())
        return false;
      if (--countIt->second < 0)
        return false;
    }

    // Fail if hte count is not equal to 0
    for (auto it = count.begin(); it != count.end(); ++it)
      if (it->second != 0)
        return false;

    return true;
  }
}

#endif // MODULE_COMBINATORY_IS_INTERLEAVED_HXX
