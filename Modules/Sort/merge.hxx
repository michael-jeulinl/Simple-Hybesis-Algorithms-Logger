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
#ifndef MODULE_SORT_MERGE_HXX
#define MODULE_SORT_MERGE_HXX

// STD includes
#include <iterator>

namespace SHA_Sort
{
  /// MergeInplace - Inplace merging of two ordered sequences of a collection
  /// Proceed a in place merge of two sequences of elements contained in [begin, middle[ and [middle, end[.
  ///
  /// @warning Both sequence [bengin, middle[ and [middle, end[ need to be ordered.
  ///
  /// @remark use MergeWithBuffer to proceed the merge using a buffer:
  /// Takes higher memory consumption and lower computation consumption.
  ///
  /// @complexity O(N * M)
  ///
  /// @tparam IT type.
  ///
  /// @param begin,middle,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename IT>
  class MergeInPlace
  {
  public:
    void operator()(IT begin, IT middle, IT end)
    {
      if (std::distance(begin, middle) < 1 || std::distance(middle, end) < 1)
        return;

      // Use first half as receiver
      for(; begin < middle; ++begin)
      {
        if (*middle >= *begin)
          continue;

        typename std::iterator_traits<IT>::value_type value;
        std::swap(value, *begin);    // keep the higher value
        std::swap(*begin, *middle);  // Place it at the beginning of the second list

        // Displace the higher value in the right place of the second list by swapping
        auto it = middle;
        for (; it != end - 1; ++it)
        {
          if (*(it + 1) >= value)
            break;

          std::swap(*it, *(it + 1));
        }

        // Restore the value at his right place
        std::swap(*it, value);
      }
    }
  };


  /// MergeWithBuffer - Merging of two ordered sequences of a collection using intermediate buffer
  /// Proceed a merge of two sequences of elements contained in [begin, middle[ and [middle, end[.
  ///
  /// @warning Both sequence [bengin, middle[ and [middle, end[ need to be ordered.
  ///
  /// @remark use MergeInPlace to proceed the merge in place:
  /// Takes lower memory consumption and higher computation consumption.
  ///
  /// @complexity O(N).
  ///
  /// @tparam IT type.
  ///
  /// @param begin,middle,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Container, typename IT>
  class MergeWithBuffer
  {
  public:
    void operator()(IT begin, IT middle, IT end)
    {
      if (std::distance(begin, middle) < 1 || std::distance(middle, end) < 1)
        return;

      Container buffer;
      buffer.resize(std::distance(begin, end)); // Reserve right space (e.g. string container)
      auto buffIt = buffer.begin();
      auto tmpBegin = begin;

      // Merge into the buffer array taking one by one the lowest sequence element
      const auto curMiddle(middle);
      while (begin != curMiddle && middle != end)
      {
        if (*begin <= *middle)
          *buffIt++ = *begin++;
        else
          *buffIt++ = *middle++;
      }

      // Finish remaining list into the buffer
      for (; begin != curMiddle; ++buffIt, ++begin)
        *buffIt = *begin;
      for (; middle != end; ++buffIt, ++middle)
        *buffIt = *middle;

      // Refill array given the right position
      for (buffIt = buffer.begin(); buffIt != buffer.end(); ++buffIt, ++tmpBegin)
        *tmpBegin = *buffIt;
    }
  };

  /// MergeSort - John von Neumann in 1945
  /// Proceed merge-sort on the elements whether using an in-place strategy or using a buffer.
  ///
  /// @complexity O(N * log(N)).
  ///
  /// @tparam IT type using to go through the collection.
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  /// @todo add compare template functor
  template <typename Container, typename IT, typename Aggregator = MergeWithBuffer<Container, IT>>
  void MergeSort(const IT& begin, const IT& end)
  {
    const auto ksize = static_cast<const int>(std::distance(begin, end));
    if (ksize < 2)
      return;

    auto middle = begin + ksize / 2;

    // Recursively break the vector into two pieces
    MergeSort<Container, IT, Aggregator>(begin, middle);
    MergeSort<Container, IT, Aggregator>(middle, end);

    // Merge the two pieces
    Aggregator()(begin, middle, end);
  }
}

#endif // MODULE_SORT_MERGE_HXX
