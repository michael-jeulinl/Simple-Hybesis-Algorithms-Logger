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
  /// @tparam Iterator type using to go through the collection.
  /// @tparam Compare functor type (std::less_equal for smaller elements in left partition,
  /// std::greater_equal for greater elements in left partition).
  ///
  /// @param begin,end const iterators to the initial and final positions of
  /// the sequence to be pivoted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param pivot iterators to position between begin and end.
  ///
  /// @return void.
  template <typename Iterator, typename Compare /*= std::less_equal*/>
  void Partition(const Iterator& begin, Iterator& pivot, const Iterator& end)
  {
    if (std::distance(begin, end) < 2 || pivot == end)
      return;

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

    pivot = store;                 // Return the pointer on the pivot
    std::swap(*(end - 1), *pivot); // Replace the pivot at its good position
  }
};

#endif() // MODULE_SORT_PARTITION_HXX
