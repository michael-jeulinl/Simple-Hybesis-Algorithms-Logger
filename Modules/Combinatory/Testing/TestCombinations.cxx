#include <gtest/gtest.h>
#include <combinations.hxx>

using namespace SHA_Combinatory;

#ifndef DOXYGEN_SKIP
namespace {
  const int SmallIntArray[] = {2, 1, 3};  // Small array containing 2, 1, 3 values

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
  typedef std::list<Container_Type> Output_Container_Type;
}
#endif /* DOXYGEN_SKIP */

// Test combinations
TEST(TestCombinations, Combinations)
{
  // Empty vector - no combinations
  {
    const Container_Type kEmptyCollection = Container_Type();
    Output_Container_Type combinations = Combinations<Container_Type, Const_Iterator_Type>
      (kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_EQ(0, combinations.size());
  }

  // Inversed iterator - no combinations
  {
    const Container_Type kUnicCollection = Container_Type(1, 10);
    Output_Container_Type combinations = Combinations<Container_Type, Const_Iterator_Type>
      (kUnicCollection.end(), kUnicCollection.begin());
    EXPECT_EQ(0, combinations.size());
  }

  // Unic element vector - Unique object returned as combinations
  {
    const Container_Type kUnicCollection = Container_Type(1, 10);
    Output_Container_Type combinations = Combinations<Container_Type, Const_Iterator_Type>
      (kUnicCollection.begin(), kUnicCollection.end());
    EXPECT_EQ(1, combinations.size());
    EXPECT_EQ(1, combinations.begin()->size());
    EXPECT_EQ(10, *(combinations.begin()->begin()));
  }

  // Run with value 2, 1, 3
  {
    const Container_Type kSmallArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) /
      sizeof(Container_Type::value_type));
    Output_Container_Type combinations = Combinations<Container_Type, Const_Iterator_Type>
      (kSmallArray.begin(), kSmallArray.end());
    EXPECT_EQ(7, combinations.size());

    int countEls = 0;
    for (Output_Container_Type::const_iterator it = combinations.begin(); it != combinations.end(); ++it)
      countEls += static_cast<int>(it->size());
    EXPECT_EQ(12, countEls);
    //@TODO check sequence by sequence? (non ordered)
  }

  // String run
  {
    const std::string abcStr = "abc";
    std::list<std::string> combinations =
      Combinations<std::string, std::string::const_iterator>(abcStr.begin(), abcStr.end());
    EXPECT_EQ(7, combinations.size());

    int countEls = 0;
    for (std::list<std::string>::const_iterator it = combinations.begin(); it != combinations.end(); ++it)
      countEls += static_cast<int>(it->size());
    EXPECT_EQ(12, countEls);
    //@TODO check sequence by sequence? (non ordered)
  }
}
