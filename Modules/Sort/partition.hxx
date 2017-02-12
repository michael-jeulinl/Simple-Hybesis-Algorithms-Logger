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
#ifndef MODULE_SORT_PARTITION_HXX
#define MODULE_SORT_PARTITION_HXX

// STD includes
#include <iterator>

namespace SHA_Sort
{
  /// Partition-Exchange
  /// Proceed an in-place patitionning on the elements.
  ///
  /// @complexity O(N).
  ///
  /// @tparam IT type using to go through the collection.
  /// @tparam Compare functor type (std::less_equal for smaller elements in left partition,
  /// std::greater_equal for greater elements in left partition).
  ///
  /// @param begin,end const iterators to the initial and final positions of
  /// the sequence to be pivoted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param pivot iterator on which the partition is delimited between begin and end.
  ///
  /// @return new pivot iterator.
  template <typename IT, typename Compare = std::less_equal<typename std::iterator_traits<IT>::value_type>>
  IT Partition(const IT& begin, const IT& pivot, const IT& end)
  {
    if (std::distance(begin, end) < 2 || pivot == end)
      return pivot;

    auto pivotValue = *pivot;       // Keep the pivot value;
    std::swap(*pivot, *(end - 1));  // Put the pivot at the end for convenience
    auto store = begin;             // Put the store pointer at the beginning

    // Swap each smaller before the pivot item
    for (auto it = begin; it != end - 1; ++it)
    {
      if (Compare()(*it, pivotValue))
      {
        std::swap(*store, *it);
        ++store;
      }
    }

    // Replace the pivot at its good position
    std::swap(*(end - 1), *store);

    return store;
  }
}

#endif // MODULE_SORT_PARTITION_HXX
