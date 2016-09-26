#ifndef MODULE_SEARCH_MAX_SUB_SEQUENCE_HXX
#define MODULE_SEARCH_MAX_SUB_SEQUENCE_HXX

// STD includes
#include <iterator>
#include <utility>

namespace SHA_Search
{
  /// Max Sub Sequence
  /// Identify the subarray with the maximum/minimum sum.
  ///
  /// @details The algorithm can be seen as an elicitation of the MaxDistance one.
  /// One of the problem resolved by this algorithm is:
  /// "Given an array of gains/losses over time, find the period that represents the best/worst
  /// cummulative gain."
  /// The algorithm uses the fact that the sum operator is a transitive function.
  ///
  /// @complexity O(N * (O(f(a, b)) + O(g(a, b))) with:
  /// - f(a, b) the distance functor used; is O(1) for the default std::minus.
  /// - g(a, b) the compare functor used; is O(1) for the default std::greater.
  ///
  /// @tparam Iterator type using to go through the collection.
  /// @tparam Distance functor type computing the distance between two elements.
  /// @tparam Compare functor type.
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @todo return Iterators instead of indexes.
  ///
  /// @return indexes of the array with the maximum/minimum sum, <-1,-1> in case of error.
  template <typename Iterator, typename Distance /*= std::minus*/, typename Compare /*= std::greater*/>
  std::pair<int, int> MaxSubSequence(const Iterator& begin, const Iterator& end)
  {
    if (std::distance(begin, end) < 2)
      return std::pair<int, int>(-1, -1);

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, minValIdx);
    auto minSum = static_cast<Iterator::value_type>(0);
    auto currSum = *begin;
    auto maxSum = *begin;

    for (auto it = begin + 1; it != end; ++it)
    {
      currSum += *it;
      auto currentIdx = static_cast<int>(std::distance(begin, it));

      // keep track of the minimum sum and its first value index
      if (Compare()(minSum, currSum))
      {
        minValIdx = currentIdx;
        minSum = currSum;
      }

      // Keeps track of the maximal sub array and its end value index
      auto curMax = Distance()(currSum, minSum);
      if (Compare()(curMax, maxSum))
      {
        indexes.first = minValIdx + ((*(begin + minValIdx) < 0) ? 1 : 0);
        indexes.second = currentIdx;
        maxSum = Distance()(currSum, minSum);
      }
    }

    return indexes;
  }
};

#endif() // MODULE_SEARCH_MAX_SUB_SEQUENCE_HXX
