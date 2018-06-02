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
#ifndef MODULE_SEARCH_MAX_KTH_ELEMENT_HXX
#define MODULE_SEARCH_MAX_KTH_ELEMENT_HXX

#include <Sort/partition.hxx>

// STD includes
#include <iterator>

namespace huc
{
  namespace search
  {
    /// Kt'h Order Statitstic - Find the kth smallest/biggest element contained within [begin, end[.
    ///
    /// @warning this method is not stable (does not keep order with element of the same value).
    /// @warning this method changes the elements order between your iterators.
    ///
    /// @tparam IT Random-access iterator type.
    /// @tparam Compare functor type (std::less_equal to find kth smallest element,
    /// std::greater_equal to find the kth biggest one).
    ///
    /// @param begin,end - ITs to the initial and final positions of
    /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
    /// first and last, including the element pointed by first but not the element pointed by last.
    /// @param k the zero-based kth element - 0 for the biggest/smallest.
    ///
    /// @return the kth smallest IT element of the array, the end IT in case of failure.
    template <typename IT, typename Compare = std::less_equal<typename std::iterator_traits<IT>::value_type>>
    IT KthOrderStatistic(const IT& begin, const IT& end, unsigned int k)
    {
      // Sequence does not contain enough elements: Could not find the k'th one.
      const auto kSize = static_cast<const int>(std::distance(begin, end));
      if (k >= static_cast<unsigned int>(kSize))
        return end;

      auto pivot = begin + (rand() % kSize);                           // Take random pivot
      auto newPivot = sort::Partition<IT, Compare>(begin, pivot, end); // Partition

      // Get the index of the pivot (i'th value)
      const auto kPivotIndex = std::distance(begin, newPivot);

      // If at the k'th position: found!
      if (kPivotIndex == k)
        return newPivot;

      // Recurse search on left part if there is more than k elements within the left sequence
      // Recurse search on right otherwise
      return (kPivotIndex > k) ? KthOrderStatistic<IT, Compare>(begin, newPivot, k)
                               : KthOrderStatistic<IT, Compare>(newPivot, end, k - kPivotIndex);

    }
  }
}

#endif // MODULE_SEARCH_MAX_KTH_ELEMENT_HXX
