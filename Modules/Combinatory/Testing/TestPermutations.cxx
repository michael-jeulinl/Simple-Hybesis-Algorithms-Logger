#include <gtest/gtest.h>
#include <permutations.hxx>

using namespace SHA_Combinatory;

#ifndef DOXYGEN_SKIP
namespace {
  const int SmallIntArray[] = {2, 1, 3};  // Small array containing 2, 1, 3 values

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
  typedef std::list<Container_Type> Output_Container_Type;
}
#endif /* DOXYGEN_SKIP */

// Test permutations
TEST(TestCore, Permutations)
{
  // Empty vector - no permutations
  {
    const Container_Type kEmptyCollection = Container_Type();
    Output_Container_Type permutations = Permutations<Container_Type, Const_Iterator_Type>
      (kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_EQ(0, permutations.size());
  }

  // Inversed iterator - no permutations
  {
    const Container_Type kUnicCollection = Container_Type(1,10);
    Output_Container_Type permutations = Permutations<Container_Type, Const_Iterator_Type>
      (kUnicCollection.end(), kUnicCollection.begin());
    EXPECT_EQ(0, permutations.size());
  }

  // Unic element vector - Unique object returned as permutation
  {
    const Container_Type kUnicCollection = Container_Type(1,10);
    Output_Container_Type permutations = Permutations<Container_Type, Const_Iterator_Type>
      (kUnicCollection.begin(), kUnicCollection.end());
    EXPECT_EQ(1, permutations.size());
    EXPECT_EQ(1, permutations.begin()->size());
    EXPECT_EQ(10, *(permutations.begin()->begin()));
  }

  // Same elements vector - n! list of n equivalent elements
  {
    const Container_Type kSameElCollection = Container_Type(3,10);
    Output_Container_Type permutations = Permutations<Container_Type, Const_Iterator_Type>
      (kSameElCollection.begin(), kSameElCollection.end());
    EXPECT_EQ(6, permutations.size());
    for (Output_Container_Type::const_iterator it = permutations.begin(); it != permutations.end(); ++it)
    {
      EXPECT_EQ(3, it->size());
      for (Container_Type::const_iterator itEl = it->begin(); itEl != it->end(); ++itEl)
        EXPECT_EQ(10, *itEl);
    }
  }

  // Run with value 2, 1, 3
  {
    const Container_Type kSmallArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) /
      sizeof(Container_Type::value_type));
    Output_Container_Type permutations = Permutations<Container_Type, Const_Iterator_Type>
      (kSmallArray.begin(), kSmallArray.end());
    EXPECT_EQ(6, permutations.size());
    for (Output_Container_Type::const_iterator it = permutations.begin(); it != permutations.end(); ++it)
      EXPECT_EQ(3, it->size());
    //@TODO check sequence by sequence? (non ordered)
  }

  // String run
  {
    const std::string abcStr = "abc";
    std::list<std::string> permutations =
      Permutations<std::string, std::string::const_iterator>(abcStr.begin(), abcStr.end());
    EXPECT_EQ(6, permutations.size());
    for (std::list<std::string>::const_iterator it = permutations.begin(); it != permutations.end(); ++it)
      EXPECT_EQ(3, it->size());
    //@TODO check sequence by sequence? (non ordered)
  }
}
