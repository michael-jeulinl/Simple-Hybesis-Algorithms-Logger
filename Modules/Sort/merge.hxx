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
#ifndef MODULE_SORT_MERGE_HXX
#define MODULE_SORT_MERGE_HXX

// STD includes
#include <iterator>

namespace huc
{
  namespace sort
  {
    /// MergeInplace Functor - In-Place merging of two ordered sequences of a collection
    /// contained in [begin, middle[ and [middle, end[.
    ///
    /// @warning Both sequence [bengin, middle[ and [middle, end[ need to be ordered.
    ///
    /// @remark use MergeWithBuffer to proceed the merge using a buffer:
    /// Takes higher memory consumption and lower computation consumption.
    ///
    /// @tparam IT type using to go through the collection.
    /// @tparam Compare functor type (std::less_equal in order, std::greater_equal for inverse order).
    ///
    /// @param begin,middle,end iterators to the initial and final positions of
    /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
    /// first and last, including the element pointed by first but not the element pointed by last.
    ///
    /// @return void.
    template <typename IT, typename Compare = std::less<typename std::iterator_traits<IT>::value_type>>
    class MergeInPlace
    {
    public:
      void operator()(const IT& begin, const IT& pivot, const IT& end)
      {
        if (std::distance(begin, pivot) < 1 || std::distance(pivot, end) < 1)
          return;

        // Use first half as receiver
        for(auto curBegin = begin; curBegin < pivot; ++curBegin)
        {
          if (Compare()(*curBegin, *pivot))
            continue;

          // Place it at the beginning of the second list
          std::swap(*curBegin, *pivot);

          // Displace the higher value in the right place of the second list by swapping
          auto it = pivot;
          for (; it != end - 1; ++it)
          {
            if (Compare()(*it, *(it + 1)))
              break;

            std::swap(*it, *(it + 1));
          }
        }
      }
    };

    /// MergeWithBuffer Functor - Merging of two ordered sequences of a collection
    /// of elements contained in [begin, middle[ and [middle, end[ using intermediate buffer.
    ///
    /// @warning Both sequence [bengin, middle[ and [middle, end[ need to be ordered.
    ///
    /// @remark use MergeInPlace to proceed the merge in place:
    /// Takes lower memory consumption and higher computation consumption.
    ///
    /// @tparam IT type using to go through the collection.
    /// @tparam Compare functor type (std::less_equal in order, std::greater_equal for inverse order).
    ///
    /// @param begin,middle,end iterators to the initial and final positions of
    /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
    /// first and last, including the element pointed by first but not the element pointed by last.
    ///
    /// @return void.
    template <typename IT, typename Compare = std::less<typename std::iterator_traits<IT>::value_type>>
    class MergeWithBuffer
    {
    public:
      void operator()(IT begin, IT middle, IT end)
      {
        if (std::distance(begin, middle) < 1 || std::distance(middle, end) < 1)
          return;

        // Create buffer with appropriate space
        std::vector<typename std::iterator_traits<IT>::value_type> buffer;
        buffer.resize(std::distance(begin, end));
        auto buffIt = buffer.begin();
        auto tmpBegin = begin;

        // Merge into the buffer array taking one by one the lowest sequence element
        const auto curMiddle(middle);
        while (begin != curMiddle && middle != end)
        {
          if (Compare()(*begin, *middle))
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

    /// MergeSort - Proceed sort on the elements whether using an in-place strategy or using a buffer one.
    ///
    /// @tparam IT type using to go through the collection.
    /// @tparam Aggregator functor type used to aggregate two sorted sequences.
    ///
    /// @param begin,end iterators to the initial and final positions of
    /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
    /// first and last, including the element pointed by first but not the element pointed by last.
    ///
    /// @return void.
    template <typename IT, typename Aggregator = MergeWithBuffer<IT>>
    void MergeSort(const IT& begin, const IT& end)
    {
      const auto ksize = static_cast<const int>(std::distance(begin, end));
      if (ksize < 2)
        return;

      auto pivot = begin + ksize / 2;

      // Recursively break the vector into two pieces
      MergeSort<IT, Aggregator>(begin, pivot);
      MergeSort<IT, Aggregator>(pivot, end);

      // Merge the two pieces
      Aggregator()(begin, pivot, end);
    }
  }
}

#endif // MODULE_SORT_MERGE_HXX
