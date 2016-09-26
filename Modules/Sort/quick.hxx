#ifndef MODULE_SORT_QUICK_HXX
#define MODULE_SORT_QUICK_HXX

#include <partition.hxx>

namespace SHA_Sort
{
  /// Quick Sort - Partition-Exchange Sort
  /// Proceed an in-place quick-sort on the elements.
  ///
  /// @complexity O(N * log(N)) in average case and O(N²) on worst case.
  /// @remark: this algorithm performs in general 2 to 3 time faster than a classic merge sort.
  /// @remark: this algorithm is easily parallelizable.
  ///
  /// @tparam Iterator type using to go through the collection.
  /// @tparam Compare functor type (std::less_equal in order, std::greater_equal for inverse order).
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Iterator, typename Compare  /*= std::less_equal*/>
  void QuickSort(Iterator& begin, Iterator& end)
  {
    const auto distance = static_cast<const int>(std::distance(begin, end));
    if (distance < 2)
      return;

    auto pivot = begin + (rand() % (end - begin));   // Pick Random Pivot € [begin, end]
    Partition<Iterator, Compare>(begin, pivot, end); // Proceed partition

    QuickSort<Iterator, Compare>(begin, pivot);   // Recurse on first partition
    QuickSort<Iterator, Compare>(pivot + 1, end); // Recurse on second partition
  }
};

#endif() // MODULE_SORT_QUICK_HXX
