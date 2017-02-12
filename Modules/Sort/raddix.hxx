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
#ifndef MODULE_SORT_RADDIX_HXX
#define MODULE_SORT_RADDIX_HXX

// STD includes
#include <queue>
#include <vector>

namespace SHA_Sort
{
  /// LSD Raddix Sort - Non-comparative integer sorting algorithm
  /// Proceed a raddix-sort on the elements contained in [begin, end[
  ///
  /// @warning Works properly only with integral type of non-negative values.
  ///
  /// @complexity O(d * N) with d max number of digits.
  ///
  /// @tparam IT type using to go through the collection.
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param base base in which the numbers are represented.
  ///
  /// @return void.
  template <typename IT>
  void RaddixSort(const IT& begin, const IT& end, unsigned int base = 10)
  {
    if (std::distance(begin, end) < 2)
      return;

    // Create a bucket for each possible value of a digit
    std::vector<std::queue<typename std::iterator_traits<IT>::value_type>> buckets(base);

    // For all possible digit
    for (size_t powBase = 1;
         powBase < std::numeric_limits<typename std::iterator_traits<IT>::value_type>::max();
         powBase *= base)
    {
      // Push each number into the bucket of its digit value
      for (auto it = begin; it != end; ++it)
        buckets[static_cast<int>(*it / powBase) % base].push(*it);

      // Dequeu back all the values
      auto itSrc = begin;
      for (auto it = buckets.begin(); it != buckets.end(); ++it)
        while (!it->empty())
        {
          *(itSrc++) = it->front();
          it->pop();
        }
    }
  }
}

#endif // MODULE_SORT_RADDIX_HXX
