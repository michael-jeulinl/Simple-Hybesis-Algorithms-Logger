#ifndef MODULE_SORT_MERGE_HXX
#define MODULE_SORT_MERGE_HXX

// STD includes
#include <iterator>

namespace SHA_Sort
{
  /// MergeSort - John von Neumann in 1945
  /// Proceed merge-sort on the elements whether using an in-place strategy or using a buffer.
  ///
  /// @complexity O(N * log(N)).
  ///
  /// @tparam Iterator type using to go through the collection.
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Container, typename Iterator, typename Aggregator>
  void MergeSort(Iterator& begin, Iterator& end)
  {
    const auto ksize = static_cast<const int>(std::distance(begin, end));
    if (ksize < 2)
      return;

    Iterator middle = begin + ksize / 2;

    // Recursively break the vector into two pieces
    MergeSort<Container, Iterator, Aggregator>(begin, middle);
    MergeSort<Container, Iterator, Aggregator>(middle, end);

    // Merge the two pieces
    Aggregator()(begin, middle, end);
  }

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
  /// @tparam iterator type.
  ///
  /// @param begin,middle,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Iterator>
  class MergeInPlace
  {
  public:
    void operator()(Iterator begin, Iterator middle, Iterator end)
    {
      if (std::distance(begin, middle) < 1 || std::distance(middle, end) < 1)
        return;

      // Use first half as receiver
      for(; begin < middle; ++begin)
      {
        if (*middle >= *begin)
          continue;

        Iterator::value_type value;
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
  /// @tparam iterator type.
  ///
  /// @param begin,middle,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Container, typename Iterator>
  class MergeWithBuffer
  {
  public:
    void operator()(Iterator begin, Iterator middle, Iterator end)
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
};

#endif() // MODULE_SORT_MERGE_HXX
