#ifndef MODULE_COMBINATORY_COMBINATIONS_HXX
#define MODULE_COMBINATORY_COMBINATIONS_HXX

// STD includes
#include <list>

namespace SHA_Combinatory
{
  /// Combinations - Return all possible combinations of elements containing within the sequence.
  ///
  /// @complexity O(2^n)
  ///
  /// @remark a vector is not recommended as type for the Output_Container to avoid stack overflow as well
  /// as extra complexity due to frequent resizing (use instead structure such as list or a another with
  /// your own allocator).
  ///
  /// @tparam Iterator type using to go through the collection.
  ///
  /// @param begin,end - iterators to the initial and final positions of
  /// the sequence. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return a collection containing all possible combined subsets.
  template <typename Container, typename Iterator>
  std::list<Container> Combinations(const Iterator& begin, const Iterator& end)
  {
    std::list<Container> combinations;

    // Recusion termination
    const auto kSeqSize = static_cast<const int>(std::distance(begin, end));
    if (kSeqSize <= 0)
      return combinations;

    // Keep first element
    Container elementContainer;
    elementContainer.push_back(*begin);
    combinations.push_back(elementContainer);

    // Recursion termination
    if (kSeqSize == 1)
      return combinations;

    // Build all permutations of the suffix - Recursion
    auto subCombinations = Combinations<Container, Iterator>(begin + 1, end);

    // Put the letter into every possible position of the existing permutations.
    Container currentPermutation;
    for (auto it = subCombinations.begin(); it != subCombinations.end(); ++it)
    {
      combinations.push_back(*it);
      it->push_back(*begin);
      combinations.push_back(*it);
    }

    return combinations;
  }
};

#endif() // MODULE_COMBINATORY_COMBINATIONS_HXX
