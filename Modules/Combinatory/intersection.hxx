#ifndef MODULE_COMBINATORY_INTERSECTION_HXX
#define MODULE_COMBINATORY_INTERSECTION_HXX

// STD includes
#include <set>

namespace SHA_Combinatory
{
  /// Intersection - Return Intersection of the two sequences.
  ///
  /// @remark Retrieve the intersection of two sequences keeping dupplicate keys distinct.
  ///
  /// @complexity O(N + M).
  ///
  /// @tparam Iterator type using to go through the collection.
  ///
  /// @param beginFirst,endFirst,beginSecond,endSecond - iterators to the initial and final positions of
  /// the sequences. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return a vector containing the intersection of both sequences.
  template <typename Container, typename Iterator>
  Container Intersection(const Iterator& beginFirst, const Iterator& endFirst,
                         const Iterator& beginSecond, const Iterator& endSecond)
  {
    // Take the smallest sequence for initial count
    const auto kFirstSize = std::distance(beginFirst, endFirst);
    const auto kSecondSize = std::distance(beginSecond, endSecond);
    const bool kIsFirstSmaller = (kFirstSize <= kSecondSize);

    // Create and set enough capacity for the intersection
    Container intersection;
    intersection.reserve((kIsFirstSmaller) ? kFirstSize : kSecondSize);

    // Count each element of the smaller array
    std::multiset<Iterator::value_type> count;
    const auto kCountEndIt = (kIsFirstSmaller) ? endFirst : endSecond;
    for (auto it = (kIsFirstSmaller) ? beginFirst : beginSecond; it != kCountEndIt; ++it)
        count.insert(*it);

    // Push the element if find into the multiset and delete its instance from the counter
    auto kIntersectEndIt = (kIsFirstSmaller) ? endSecond : endFirst;
    for (auto it = (kIsFirstSmaller) ? beginSecond : beginFirst; it != kIntersectEndIt; ++it)
    {
      // Move element from count to intersection if found
      auto foundIt = count.find(*it);
      if (foundIt != count.end())
      {
        intersection.push_back(*it);
        count.erase(foundIt);
      }
    }

    return intersection;
  }
};

#endif() // MODULE_COMBINATORY_INTERSECTION_HXX
