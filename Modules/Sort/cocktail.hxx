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
#ifndef MODULE_SORT_COKTAIL_HXX
#define MODULE_SORT_COKTAIL_HXX

// STD includes
#include <iterator>

namespace SHA_Sort
{
  /// Cocktail Sort - Cocktail Sort
  /// Proceed an in-place Cocktail-sort on the elements.
  /// Cocktail shaker sort,[1] also known as bidirectional bubble sort,[2] cocktail sort,
  /// martini sort, shaker sort (which can also refer to a variant of selection sort),
  /// ripple sort, shuffle sort,[3] or shuttle sort, is a variation of bubble sort.
  /// The algorithm differs from a bubble sort in that it sorts in both directions on each pass.
  ///
  /// @complexity O(N²) in average case and O(N²) on worst case.
  /// @remark: Although the algorithm is simple and an optimization of the bubble sort,
  /// it is too slow and impractical for most problem.
  ///
  /// @tparam IT type using to go through the collection.
  /// @tparam Compare functor type (std::less_equal in order, std::greater_equal for inverse order).
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename IT, typename Compare = std::less<typename std::iterator_traits<IT>::value_type>>
  void Cocktail(const IT& begin, const IT& end)
  {
    const auto distance = static_cast<const int>(std::distance(begin, end));
    if (distance < 2)
      return;

    int beginIdx = 0;
    int endIdx = distance - 1;
    bool hasSwapped = true;
    while (hasSwapped && beginIdx < distance - 1)
    {
      hasSwapped = false;
      // for each element from beginning - bubble it up until the end.
      for (auto it = begin + beginIdx; it < begin + endIdx; ++it)
        if (Compare()(*(it + 1), *it))
        {
          std::swap(*it, *(it + 1));
          hasSwapped = true;
        }
      --endIdx;

      if (!hasSwapped)
        break;

      // for each element from the end- bubble it down until the beginning.
      for (auto it = begin + endIdx - 1; it >= begin + beginIdx; --it)
        if (Compare()(*(it + 1), *it))
        {
          std::swap(*it, *(it + 1));
          hasSwapped = true;
        }
      ++beginIdx;
    }
  }
}

#endif // MODULE_SORT_COKTAIL_HXX
