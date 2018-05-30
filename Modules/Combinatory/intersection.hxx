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
#ifndef MODULE_COMBINATORY_INTERSECTION_HXX
#define MODULE_COMBINATORY_INTERSECTION_HXX

// STD includes
#include <set>

namespace HUC_Combinatory
{
  /// Intersection - Return Intersection of the two sequences.
  ///
  /// @remark Retrieve the intersection of two sequences keeping dupplicate keys distinct.
  ///
  /// @complexity O(N + M).
  ///
  /// @tparam IT type using to go through the collection.
  ///
  /// @param beginFirst,endFirst,beginSecond,endSecond - iterators to the initial and final positions of
  /// the sequences. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return a vector containing the intersection of both sequences.
  template <typename Container, typename IT>
  Container Intersection(const IT& beginFirst, const IT& endFirst,
                         const IT& beginSecond, const IT& endSecond)
  {
    // Take the smallest sequence for initial count
    const auto kFirstSize = std::distance(beginFirst, endFirst);
    const auto kSecondSize = std::distance(beginSecond, endSecond);
    const bool kIsFirstSmaller = (kFirstSize <= kSecondSize);

    // Create and set enough capacity for the intersection
    Container intersection;
    intersection.reserve((kIsFirstSmaller) ? kFirstSize : kSecondSize);

    // Count each element of the smaller array
    std::multiset<typename std::iterator_traits<IT>::value_type> count;
    const auto kCountEndIt = (kIsFirstSmaller) ? endFirst : endSecond;
    for (auto it = (kIsFirstSmaller) ? beginFirst : beginSecond; it != kCountEndIt; ++it)
        count.insert(*it);

    // Push the element if find into the multiset and delete its instance from the counter
    auto kIntersectEndIt = (kIsFirstSmaller) ? endSecond : endFirst;
    for (auto it = (kIsFirstSmaller) ? beginSecond : beginFirst; it != kIntersectEndIt; ++it)
    {
      // Move element from count to intersection if found
      auto foundIt = count.find(*it);
      if (foundIt != count.end())
      {
        intersection.push_back(*it);
        count.erase(foundIt);
      }
    }

    return intersection;
  }
}

#endif // MODULE_COMBINATORY_INTERSECTION_HXX
