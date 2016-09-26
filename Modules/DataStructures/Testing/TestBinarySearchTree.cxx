#include <gtest/gtest.h>
#include <binary_search_tree.hxx>

#include <functional>
#include <list>

using namespace SHA_DataStructures;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};     // Other  random array of integers with negative values
  const int SmallIntArray[] = {2, 1, 3};                             // Small array containing 2, 1, 3 values
  const int SmallIntArraySorted[] = {1, 2, 3};                       // Small array ordered containing 1, 2, 3 values

  template <typename T>
  struct EQUIVALENT
  {
    bool operator() (const T& a, const T& b) const
      { return std::abs(a - b) < std::numeric_limits<T>::epsilon(); }
  };

  template <typename T>
  struct EQUAL
  {
    bool operator() (const T& a, const T& b) const { return a == b; }
  };

  typedef std::vector<int> Container_Type;
  typedef Container_Type::value_type Value_type;
  typedef Container_Type::iterator Iterator_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
  typedef BST<Const_Iterator_Type, std::less_equal<int>, EQUAL<int>> Const_BST_type;
  typedef std::unique_ptr<Const_BST_type> Const_BST_Owner_Type;
}
#endif /* DOXYGEN_SKIP */

// Test BST Construction
TEST(TestBST, build)
{
  // Empty Array - No BST should be built
  {
    const Container_Type kEmptyCollection = Container_Type();
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_FALSE(tree);
  }

  // Begin After End - No BST should be built
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.end(), kSmallIntArray.begin());
    EXPECT_FALSE(tree);
  }

  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(2, tree->GetData());
    EXPECT_TRUE(tree->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild() == nullptr);
  }

  // Basic construction
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());

    EXPECT_EQ(2, tree->GetData());
    EXPECT_EQ(1, tree->GetLeftChild()->GetData());
    EXPECT_EQ(3, tree->GetRightChild()->GetData());
    EXPECT_TRUE(tree->GetLeftChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetLeftChild()->GetRightChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetRightChild() == nullptr);
  }

  // Basic construction
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());

    EXPECT_EQ(2, tree->GetData());
    EXPECT_EQ(1, tree->GetLeftChild()->GetData());
    EXPECT_EQ(3, tree->GetRightChild()->GetData());
    EXPECT_TRUE(tree->GetLeftChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetLeftChild()->GetRightChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetRightChild() == nullptr);
  }
}

// Test BST Construction From Sorted sequence
TEST(TestBST, buildFromSorted)
{
  // Empty Array - No BST should be built
  {
    const Container_Type kEmptyCollection = Container_Type();
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_FALSE(tree);
  }

  // Begin After End - No BST should be built
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.end(), kSmallIntArray.begin());
    EXPECT_FALSE(tree);
  }

  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(2, tree->GetData());
    EXPECT_TRUE(tree->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild() == nullptr);
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());

    EXPECT_EQ(2, tree->GetData());
    EXPECT_EQ(1, tree->GetLeftChild()->GetData());
    EXPECT_EQ(3, tree->GetRightChild()->GetData());
    EXPECT_TRUE(tree->GetLeftChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetLeftChild()->GetRightChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetRightChild() == nullptr);
  }
}

// Test BST Validity checker
TEST(TestBST, isValid)
{
  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_TRUE(tree->IsValid());
  }

  // Basic construction
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    EXPECT_TRUE(tree->IsValid());
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());
    EXPECT_TRUE(tree->IsValid());
  }

  // Wrong construction on unsorted array
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallIntArray.begin(), kSmallIntArray.end());
    EXPECT_FALSE(tree->IsValid());
  }

  // Basic construction with negative values and dupplicates
  {
    const Container_Type kRandIntArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kRandIntArray.begin(), kRandIntArray.end());
    EXPECT_TRUE(tree->IsValid());
  }
}

// Test BST Append method
TEST(TestBST, AppendTree)
{
  // Test adding some element within the BST and its structure
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(2, tree->GetData());
    tree->Insert(10);
    EXPECT_EQ(10, tree->GetRightChild()->GetData());
    tree->Insert(15);
    EXPECT_EQ(15, tree->GetRightChild()->GetRightChild()->GetData());
    tree->Insert(-10);
    EXPECT_EQ(-10, tree->GetLeftChild()->GetData());
    tree->Insert(0);
    EXPECT_EQ(0, tree->GetLeftChild()->GetRightChild()->GetData());
  }
}

// Test BST Size computation
TEST(TestBST, Size)
{
  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(1, tree->Size());
  }

  // Basic construction - 3
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    EXPECT_EQ(kSmallIntArray.size(), tree->Size());
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());
    EXPECT_EQ(kSmallSorted.size(), tree->Size());
  }

  // Basic construction with negative values and dupplicates
  {
    const Container_Type kRandIntArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kRandIntArray.begin(), kRandIntArray.end());
    EXPECT_EQ(kRandIntArray.size(), tree->Size());
  }
}

// Test Minimal Height computation
TEST(TestBST, MinHeight)
{
  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(1, tree->MinHeight());
  }

  // Basic construction
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    EXPECT_EQ(2, tree->MinHeight());
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());
    EXPECT_EQ(2, tree->MinHeight());
  }

  // Basic construction with negative values and dupplicates
  {
    const Container_Type kRandIntArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kRandIntArray.begin(), kRandIntArray.end());
    EXPECT_EQ(2, tree->MinHeight());
  }
}

// Test Minimal Height computation
TEST(TestBST, MaxHeight)
{
  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(1, tree->MaxHeight());
  }

  // Basic construction
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    EXPECT_EQ(2, tree->MaxHeight());
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());
    EXPECT_EQ(2, tree->MaxHeight());
  }

  // Basic construction with negative values and dupplicates
  {
    const Container_Type kRandIntArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kRandIntArray.begin(), kRandIntArray.end());
    EXPECT_EQ(6, tree->MaxHeight());
  }
}

// Test tree balancement
TEST(TestBST, IsBalanced)
{
  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_TRUE(tree->IsBlanced());
  }

  // Basic construction
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    EXPECT_TRUE(tree->IsBlanced());
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());
    EXPECT_TRUE(tree->IsBlanced());
  }

  // Basic construction with negative values and dupplicates
  {
    const Container_Type kSortedArrayInt(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSortedArrayInt.begin(), kSortedArrayInt.end());
    EXPECT_FALSE(tree->IsBlanced());
  }
}

// Test Finding key
TEST(TestBST, Find)
{
  // Unique element - Unique element should be found - not others
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(2, tree->Find(2)->GetData());
    EXPECT_FALSE(tree->Find(0));
    EXPECT_FALSE(tree->Find(5));
  }

  // Basic construction - Elements should be found - not others
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    EXPECT_EQ(1, tree->Find(1)->GetData());
    EXPECT_EQ(2, tree->Find(2)->GetData());
    EXPECT_EQ(3, tree->Find(3)->GetData());
    EXPECT_FALSE(tree->Find(0));
    EXPECT_FALSE(tree->Find(5));
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());
    EXPECT_EQ(1, tree->Find(1)->GetData());
    EXPECT_EQ(2, tree->Find(2)->GetData());
    EXPECT_EQ(3, tree->Find(3)->GetData());
    EXPECT_FALSE(tree->Find(0));
    EXPECT_FALSE(tree->Find(5));
  }

  // Basic construction with negative values and dupplicates - should be found
  {
    const Container_Type kRandIntArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kRandIntArray.begin(), kRandIntArray.end());
    EXPECT_EQ(-18, tree->Find(-18)->GetData());
    EXPECT_EQ(-5, tree->Find(-5)->GetData());
    EXPECT_EQ(5, tree->Find(5)->GetData());
    EXPECT_FALSE(tree->Find(1));
    EXPECT_FALSE(tree->Find(6));
  }
}

// Test Removing key
TEST(TestBST, Remove)
{
  // Empty managed object - Should not change the status
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin());
    ASSERT_TRUE(tree == nullptr);
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 2);
    EXPECT_TRUE(returnTreePtr == nullptr);
    EXPECT_TRUE(tree == nullptr);
  }

  // Unique element - Root should be erased
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 2);
    EXPECT_TRUE(returnTreePtr == nullptr);
    EXPECT_TRUE(tree == nullptr);
  }

  // Serie of same element - all nodes should be erased
  {
    const Container_Type kSmallIntArray(5, 4);
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 4);
    EXPECT_TRUE(returnTreePtr == nullptr);
    EXPECT_TRUE(tree == nullptr);
  }

  // Leaf node
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 3);

    ASSERT_TRUE(tree);
    EXPECT_EQ(tree.get(), returnTreePtr);
    EXPECT_EQ(2, tree->GetData());
    ASSERT_TRUE(tree->GetLeftChild() != nullptr);
    EXPECT_EQ(1, tree->GetLeftChild()->GetData());
    EXPECT_TRUE(tree->GetRightChild() == nullptr);
  }

  // Root node with a unique child (left)
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 2);
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 2);

    ASSERT_TRUE(tree);
    EXPECT_EQ(tree.get(), returnTreePtr);
    EXPECT_EQ(1, tree->GetData());
    EXPECT_TRUE(tree->GetRightChild() == nullptr);
    EXPECT_TRUE(tree->GetLeftChild() == nullptr);
  }

  // Root node with a unique child (right)
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin() + 1, kSmallIntArray.end());
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 1);

    ASSERT_TRUE(tree);
    EXPECT_EQ(tree.get(), returnTreePtr);
    EXPECT_EQ(3, tree->GetData());
    EXPECT_TRUE(tree->GetRightChild() == nullptr);
    EXPECT_TRUE(tree->GetLeftChild() == nullptr);
  }

  // Root node with a unique subtree child (left)
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    tree->Insert(0);
    tree->Insert(-2);
    tree->Insert(-1);
    tree->Insert(-3);
    tree->Insert(1);
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 2);

    ASSERT_TRUE(tree);
    EXPECT_EQ(tree.get(), returnTreePtr);
    EXPECT_EQ(0, tree->GetData());
    ASSERT_TRUE(tree->GetRightChild());
    EXPECT_EQ(1, tree->GetRightChild()->GetData());
    ASSERT_TRUE(tree->GetLeftChild());
    EXPECT_EQ(-2, tree->GetLeftChild()->GetData());
    EXPECT_EQ(3, tree->GetLeftChild()->Size());
  }

  // Root node with a unique subtree child (right)
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    tree->Insert(4);
    tree->Insert(6);
    tree->Insert(5);
    tree->Insert(7);
    tree->Insert(3);
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 2);

    ASSERT_TRUE(tree);
    EXPECT_EQ(tree.get(), returnTreePtr);
    EXPECT_EQ(4, tree->GetData());
    ASSERT_TRUE(tree->GetLeftChild());
    EXPECT_EQ(3, tree->GetLeftChild()->GetData());
    ASSERT_TRUE(tree->GetRightChild());
    EXPECT_EQ(6, tree->GetRightChild()->GetData());
    EXPECT_EQ(3, tree->GetRightChild()->Size());
  }

  // Root node with two childred
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 2);

    ASSERT_TRUE(tree);
    EXPECT_EQ(tree.get(), returnTreePtr);
    EXPECT_EQ(1, tree->GetData());
    ASSERT_TRUE(tree->GetRightChild());
    EXPECT_EQ(3, tree->GetRightChild()->GetData());
    EXPECT_TRUE(tree->GetLeftChild() == nullptr);
  }

  // Root node with two subtrees children
  {
    const Container_Type kContainerValue(1, 10);
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kContainerValue.begin(), kContainerValue.end());
    tree->Insert(4);
    tree->Insert(14);
    tree->Insert(2);
    tree->Insert(12);
    tree->Insert(8);
    tree->Insert(7);
    tree->Insert(15);
    tree->Insert(6);
    const Const_BST_type* returnTreePtr = Const_BST_type::Remove(tree, 10);

    ASSERT_TRUE(tree);
    EXPECT_EQ(tree.get(), returnTreePtr);
    EXPECT_EQ(8, tree->GetData());
    EXPECT_EQ(8, tree->Size());
    ASSERT_TRUE(tree->GetRightChild());
    EXPECT_EQ(3, tree->GetRightChild()->Size());
    ASSERT_TRUE(tree->GetLeftChild()->GetRightChild());
    ASSERT_EQ(7, tree->GetLeftChild()->GetRightChild()->GetData());
  }
}
