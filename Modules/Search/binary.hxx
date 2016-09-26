#ifndef MODULE_SEARCH_BINARY_HXX
#define MODULE_SEARCH_BINARY_HXX

// STD includes
#include <iterator>

namespace SHA_Search
{
  /// Binary Search
  /// Iteratively proceed a dichotomic search within a sorted collection on the first occurence
  /// of the key passed as parameter.
  ///
  /// @complexity O(log(N)).
  ///
  /// @tparam Iterator type using to go through the collection.

  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param key the key value to be searched.
  ///
  /// @return The vector index of the first found key, -1 otherwise.
  template <typename Iterator, typename T, typename IsEqual>
  int BinarySearch(const Iterator& begin, const Iterator& end, const T& key)
  {
    int index = -1;
    auto lowIt = begin;
    auto highIt = end;
    auto middleIt = lowIt + std::distance(lowIt, highIt) / 2;

    // While there is still objects between the two iterators and no object has been foud yet
    while(lowIt < highIt && index < 0)
    {
      // Found object - Set index computed from initial begin iterator
      if (IsEqual()(key, *middleIt))
        index = static_cast<int>(std::distance(begin, middleIt));
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
};

#endif() // MODULE_SEARCH_BINARY_HXX
