#ifndef MODULE_COMBINATORY_PERMUTATIONS_HXX
#define MODULE_COMBINATORY_PERMUTATIONS_HXX

// STD includes
#include <list>

namespace SHA_Combinatory
{
  /// Permutations - Return all possible permutations of elements containing within the sequence.
  ///
  /// @complexity O(N!).
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
  /// @return a collection containing all possible permuted sequences.
  template <typename Container, typename Iterator>
  std::list<Container> Permutations(const Iterator& begin, const Iterator& end)
  {
    std::list<Container> permutations; // Contains the output permutations

    // Recusion termination - Empty sequence
    const auto kSeqSize = static_cast<const int>(std::distance(begin, end));
    if (kSeqSize <= 0)
      return permutations;

    // Recusion termination - Unique element
    if (kSeqSize == 1)
    {
      Container elementContainer;
      elementContainer.push_back(*begin);
      permutations.push_back(elementContainer);
      return permutations;
    }

    // Build all permutations of the suffix - Recursion
    auto subPermutations = Permutations<Container, Iterator>(begin + 1, end);

    // Put the letter into every possible position of the existing permutations.
    Container currentPermutation;
    for (auto it = subPermutations.begin(); it != subPermutations.end(); ++it)
      for (int i = 0; i < kSeqSize; ++i)
      {
        currentPermutation = *it;
        currentPermutation.insert(currentPermutation.begin() + i, *begin);
        permutations.push_back(currentPermutation);
      }

    return permutations;
  }
};

#endif() // MODULE_COMBINATORY_PERMUTATIONS_HXX
