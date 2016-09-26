#ifndef MODULE_SEARCH_MAX_DISTANCE_HXX
#define MODULE_SEARCH_MAX_DISTANCE_HXX

// STD includes
#include <iterator>
#include <utility>

namespace SHA_Search
{
  /// MaxDistance
  /// Identifies the two indexes of the array with the maximal distance.
  ///
  /// @details Known as the simple stock market problem with the default functor (std::minus):
  /// It finds i and j that maximizes Aj – Ai, where i < j.
  /// In other words, maximizes the benefice of a resell given an array of prices varying over time.
  ///
  /// @complexity O(N * O(f(a, b))), with f(a,b) the functor used; is O(1) for the default std::minus.
  ///
  /// @tparam Iterator type using to go through the collection.
  /// @tparam Distance functor type computing the distance between two elements.
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return indexes of the array with the maximal distance, <-1,-1> in case of error.
  template <typename Iterator, typename Distance /*= std::minus*/>
  std::pair<int, int> MaxDistance(const Iterator& begin, const Iterator& end)
  {
    if (std::distance(begin, end) < 2)
      return std::pair<int, int>(-1, -1);

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, 1);
    auto maxDist = Distance()(*begin, *(begin + 1));

    for (auto it = begin + 1; it != end; ++it)
    {
      const auto currentIdx = static_cast<const int>(std::distance(begin, it));

      // Keeps track of the minimum value index
      if (*it < *(begin + minValIdx))
        minValIdx = currentIdx;

      // Keeps track of the largest distance and the indexes
      const auto distance = Distance()(*it, *(begin + minValIdx));
      if (distance > maxDist)
      {
        maxDist = distance;
        indexes.first = minValIdx;
        indexes.second = currentIdx;
      }
    }

    return indexes;
  }
};

#endif() // MODULE_COLLECTIONS_SEARCH_HXX
