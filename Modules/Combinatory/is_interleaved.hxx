#ifndef MODULE_COMBINATORY_IS_INTERLEAVED_HXX
#define MODULE_COMBINATORY_IS_INTERLEAVED_HXX

// STD includes
#include <map>

namespace SHA_Combinatory
{
  /// IsInterleaved - Return whether or not if a sequence is the interleave of the two others.
  ///
  /// @complexity O(N + M + K).
  ///
  /// @tparam Iterator type using to go through the collection.
  ///
  /// @param beginFirst,endFirst,beginSecond,endSecond,beginFull,endFull - iterators to the initial and
  /// final positions of the sequences. The range used is [first,last), which contains all the elements
  /// between first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return true if the last sequence is the interleave of the two others, false otherwise.
  template <typename Iterator>
  bool IsInterleaved(const Iterator& beginFirst, const Iterator& endFirst,
                     const Iterator& beginSecond, const Iterator& endSecond,
                     const Iterator& beginFull, const Iterator& endFull)
  {
    // Lambda that count each element occurence within the map "count"
    std::map<Iterator::value_type, int> count;
    auto lCountElement = [&count](Iterator::value_type val)
    {
      auto countIt = count.find(val);
      if (countIt != count.end())
        ++countIt->second;
      else
        count.emplace(val, 1);
    };

    // Count both sequences
    std::for_each(beginFirst, endFirst, lCountElement);
    std::for_each(beginSecond, endSecond, lCountElement);

    // Decrease the count of each element given the full sequence
    for (auto it = beginFull; it != endFull; ++it)
    {
      auto countIt = count.find(*it);
      if (countIt == count.end())
        return false;
      if (--countIt->second < 0)
        return false;
    }

    // Fail if hte count is not equal to 0
    for (auto it = count.begin(); it != count.end(); ++it)
      if (it->second != 0)
        return false;

    return true;
  }
};

#endif() // MODULE_COMBINATORY_IS_INTERLEAVED_HXX
