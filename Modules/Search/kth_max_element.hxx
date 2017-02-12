/*===========================================================================================================
 *
 * SHA - Simple Hybesis Algorithms
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms/blob/master/LICENSE
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

namespace SHA_Search
{
  /// Kth Smallest / Biggest element - Order Statitstics
  /// Find the kth smallest/biggest element contained within [begin, end[.
  ///
  /// @warning this method is not stable (does not keep order with element of the same value).
  /// @warning this method changes the elements order between your iterators.
  ///
  /// @complexity recursively look on the choosen partition:
  ///             N + N/2 + N/4 + N/8 + ... = O(N).
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
  IT MaxKthElement(const IT& begin, const IT& end, unsigned int k)
  {
    const auto kSize = static_cast<const int>(std::distance(begin, end));
    if (k >= static_cast<unsigned int>(kSize))
      return end;

    auto pivot = begin + (rand() % kSize);                               // Take random pivot
    auto newPivot = SHA_Sort::Partition<IT, Compare>(begin, pivot, end); // Partition

    // Get the index of the pivot (i'th smallest/biggest value)
    const auto kPivotIndex = std::distance(begin, newPivot);

    // Return if at the kth position
    if (kPivotIndex == k)
      return newPivot;

    // Recurse search on left part if there is more than k elements within the left sequence
    // Recurse search on right otherwise
    return (kPivotIndex > k) ? MaxKthElement<IT, Compare>(begin, newPivot, k)
                             : MaxKthElement<IT, Compare>(newPivot, end, k - kPivotIndex);

  }
}

#endif // MODULE_SEARCH_MAX_KTH_ELEMENT_HXX
