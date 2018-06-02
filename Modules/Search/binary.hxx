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
#ifndef MODULE_SEARCH_BINARY_HXX
#define MODULE_SEARCH_BINARY_HXX

// STD includes
#include <iterator>

namespace huc
{
  namespace search
  {
    /// Binary Search - Given a sorted sequence, find the exact position of a specific value.
    ///
    /// @tparam IT type using to go through the collection.
    /// @tparam Compare functor type (std::less_equal to find kth smallest element,
    /// std::greater_equal to find the kth biggest one).

    /// @param begin,end iterators to the initial and final positions of
    /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
    /// first and last, including the element pointed by first but not the element pointed by last.
    /// @param key the key value to be searched.
    ///
    /// @return The index of the first key occurence found, -1 if not found.
    template <typename IT,
              typename IsEqual = std::equal_to<typename std::iterator_traits<IT>::value_type>>
    int BinarySearch(const IT& begin, const IT& end, const typename std::iterator_traits<IT>::value_type& key)
    {
      int index = -1;
      auto lowIt = begin;
      auto highIt = end;
      auto middleIt = lowIt + std::distance(lowIt, highIt) / 2;

      // While there is still objects between the two ITs and no object has been foud yet
      while(lowIt < highIt && index < 0)
      {
        // Found object - Set index computed from initial begin IT
        if (IsEqual()(key, *middleIt))
        {
          index = static_cast<int>(std::distance(begin, middleIt));
          break;
        }
        // Search key within upper collection
        else if (key > *middleIt)
          lowIt = middleIt + 1;
        // Search key within lower collection
        else
          highIt = middleIt;

        middleIt = lowIt + std::distance(lowIt, highIt) / 2;
      }

      return index;
    }
  }
}

#endif // MODULE_SEARCH_BINARY_HXX
