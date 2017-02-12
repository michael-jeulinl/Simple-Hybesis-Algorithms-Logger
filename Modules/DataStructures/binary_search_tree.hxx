/*===========================================================================================================
 *
 * SHA - Simple Hybesis Algorithms
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#ifndef MODULE_DATA_STRUCTURES_BST_HXX
#define MODULE_DATA_STRUCTURES_BST_HXX

#include <memory>

namespace SHA_DataStructures
{
  /// @class BST
  ///
  /// A Binary Search Tree, Ordered Tree or Sorted Binary Tree divides all its sub-trees into
  /// two segments left sub-tree and right sub-tree that follow these rules:
  /// - The left sub-tree of a node has key that respect the Compare operator (e.g. less_equal) with
  ///   its parent node's key.
  /// - The right sub-tree of a node has key that does not respect the Compare operator with
  ///   its parent node's key.
  /// - Duplicates key are inserted of the left sub-tree with respect to the Compare operator.
  ///
  /// It keeps their keys in sorted order, so that lookup and other operations can use the
  /// principle of binary search: when looking for a key in a tree (or a place to insert a new key),
  /// they traverse the tree from root to leaf, making comparisons to keys stored in the nodes of the tree
  /// and deciding, based on the comparison, to continue searching in the left or right subtrees.
  ///
  /// @advantages
  /// - Memory efficient and managed.
  /// - Use principe of binary search for insert, delete and lookup operations.
  /// - Represent well hierarchies.
  /// - Get all keys in sorted order by just doing Inorder Traversal.
  /// - Doing order statistics, closest lower/greater elements, range queries etc. operations are easy.
  ///
  /// @drawbacks
  /// - The shape of the binary search tree depends entirely on the order of insertions and
  ///   deletions, and can become degenerate.
  /// - When inserting or searching for an element in a binary search tree, the key of each visited node has
  ///   to be compared with the key of the element to be inserted or found.
  template <typename IT, typename Compare, typename IsEqual>
  class BST
  {
    typedef typename std::iterator_traits<IT>::value_type Value;
    public:
      /// Build - Construct in a naive way a Binary Search Tree given an unordered sequence of elements.
      ///
      /// @param begin,end - ITs to the initial and final positions of
      /// the sequence used to build the tree. The range used is [first,last), which contains
      /// all the elements between first and last, including the element pointed by first but
      /// not the element pointed by last.
      ///
      /// @complexity O(n * m).
      ///
      /// @return Binary Search Tree pointer to be owned, nullptr if construction failed.
      static std::unique_ptr<BST> Build(const IT& begin, const IT& end)
      {
        if (begin >= end)
          return  nullptr;

        // Create root node
        auto root = std::unique_ptr<BST>(new BST(*begin));

        // Insert all remaining elements within the tree
        for (auto it = begin + 1; it != end; ++it)
          root->Insert(*it);

        return root;
      }

      /// BuildFromSorted - Construct a Balanced Binary Search Tree given an ordered sequence of elements.
      ///
      /// @param begin,end - ITs to the initial and final positions of
      /// the sequence used to build the tree. The range used is [first,last), which contains
      /// all the elements between first and last, including the element pointed by first but
      /// not the element pointed by last.
      ///
      /// @complexity O(n).
      ///
      /// @warning the algorithm does not check the validity on data order; using this algorithm with
      /// unordored data will most likely result in an invalid BST. (Can be checked using IsValid method).
      ///
      /// @warning if the compare template argument is inverting the BST (e.g greater_equal), the sequence
      /// should be sorted given the same paradigme (e.g. greater value first).
      ///
      /// @return Binary Search Tree pointer to be owned, nullptr if construction failed.
      static std::unique_ptr<BST> BuildFromSorted(const IT& begin, const IT& end)
      {
        if (begin >= end)
          return nullptr;

        const auto middle = begin + (std::distance(begin,end) / 2);
        auto root = std::unique_ptr<BST>(new BST(*middle));

        // Recursively insert both children
        root->SetLeftChild(std::move(BuildFromSorted(begin, middle)));
        root->SetRightChild(std::move(BuildFromSorted(middle + 1, end)));

        return root;
      }

      /// Find the first a binary search tree for a specific key.
      ///
      /// @complexity O(h), where h may be n in worst case balancement. Equal to log(n) with a balanced tree.
      ///
      /// @param data, data value to be found within the current BST.
      ///
      /// @remark this method may not find the data within an invalid binary search tree (cf. IsValid).
      ///
      /// @return first BST node matching the data.
      const BST* Find(const Value& data)
      {
        // Key found returns node
        if (IsEqual()(this->data, data))
          return this;

        // Key is less than current node - search in left subtree
        if (Compare()(data, this->data))
          return this->leftChild ? this->leftChild->Find(data) : nullptr;

        // Search in right subtree
        return this->rightChild ? this->rightChild->Find(data) : nullptr;
      }

      /// Append a new Binary Search Tree node at the right position with current value.
      ///
      /// @complexity O(n).
      ///
      /// @param data data value to be added to the current BST. Member type Value is the type of the
      /// elements in the BST, defined as an alias of its first template parameter Value
      /// (IT::Value).
      ///
      /// @return void.
      void Insert(const Value& data)
      {
        // Key is lower or equal than current root - Insert on the left side
        if (Compare()(data, this->data))
        {
          if (!this->leftChild)
            this->SetLeftChild(std::move(std::unique_ptr<BST>(new BST(data))));
          else
            this->leftChild->Insert(data);
        }
        // Key is greater than current root - Insert on the right side
        else
        {
          if (!this->rightChild)
            this->SetRightChild(std::move(std::unique_ptr<BST>(new BST(data))));
          else
            this->rightChild->Insert(data);
        }
      }

      /// Check if the Binary Search Tree is balanced.
      /// Compare the smallest branch to the biggest one to determine the balancement.
      ///
      /// @return wheter or not the tree is balanced (true) or not (false).
      bool IsBlanced() const { return this->MaxHeight() - this->MinHeight() <= 1; }

      /// Check validity of the Binary Search Tree.
      /// Recursively check if subtrees do not violate any of the rules defined by a BST.
      ///
      /// @remark Using a preordering traversal, it makes sure that:
      /// - If the node is the left child of its parent, then it must be smaller than (or equal to)
      ///   the parent and it must pass down the value from its parent to its right subtree to make sure none
      ///   of the nodes in that subtree is greater than the parent.
      /// - If the node is the right child of its parent, then it must be larger than the parent and it must
      ///   pass down the value from its parent to its left subtree to make sure none of the nodes in that
      ///   subtree is lesser than the parent.
      ///
      /// @return wheter or not the tree is a valid Binary Search Tree (true) or not (false).
      bool IsValid() const
      {
        auto previousNode = std::unique_ptr<const BST*>(new const BST*);
        *previousNode = nullptr;
        return this->IsValid(previousNode);
      }

      /// Returns the biggest branch height.
      ///
      /// Complexity O(n).
      ///
      /// @return biggest branch height composing the tree.
      std::size_t MaxHeight() const
      {
        return 1 + std::max(((this->leftChild) ? this->leftChild->MaxHeight() : 0),
                            ((this->rightChild) ? this->rightChild->MaxHeight() : 0));
      }

      /// Returns the smallest branch height.
      ///
      /// Complexity O(n).
      ///
      /// @return smallest branch height composing the tree.
      std::size_t MinHeight() const
      {
        return 1 + std::min(((this->leftChild) ? this->leftChild->MinHeight() : 0),
                            ((this->rightChild) ? this->rightChild->MinHeight() : 0));
      }

      /// Removes all elements equal [IsEqual() template parameter] to the value from the BST.
      ///
      /// @param bst the unique_ptr owning the bst on which the removal occurs.
      /// @param data to be removed from the BST. All elements with a value equivalent (IsEqual template
      /// parameter) to this are removed from the container.
      /// Member type Value is the type of the elements in the BST, defined as an alias of its first
      /// template parameter Value (IT::Value).
      ///
      /// @warning this method is destructive and may delete the bst owned by the unique_ptr. Return value
      /// may be used for inline checking as:
      /// (!Remove(bst, data)) ? tree no longer exist : tree still contains node
      ///
      /// @return the pointer handler by the bst passed as argument, nullptr if bst has been erased (empty).
      static const BST* Remove(std::unique_ptr<BST>& bst, const Value& data)
      {
        // Break on empty unique_ptr
        if (!bst)
          return nullptr;

        // Reach node matching the value
        if (!IsEqual()(bst->data, data))
        {
          if (Compare()(data, bst->data))
            Remove(bst->leftChild, data);
          else
            Remove(bst->rightChild, data);
        }
        // Proceed removal
        else
        {
          // Recursively delete all subnodes containing the same value
          if (bst->leftChild && IsEqual()(bst->data, bst->leftChild->data))
            Remove(bst->leftChild, data);

          // No child - Simply remove node
          if (!bst->leftChild && !bst->rightChild)
            bst.reset();
          // Both children:
          // - Swap node value with its predecessor
          // - Remove predecessor node and replace it with its child
          else if (bst->leftChild && bst->rightChild)
          {
            auto& predecessor = bst->GetPredecessor();
            std::swap(bst->data, predecessor->data);
            predecessor.reset(predecessor->leftChild.release());
          }
          // Left node is unique child - remove node and replace it with its child.
          else if (bst->leftChild)
            bst.reset(bst->leftChild.release());
          // Right node is unique child - remove node and replace it with its child.
          else if (bst->rightChild)
            bst.reset(bst->rightChild.release());
        }

        // Return pointer handled by bst.
        return bst.get();
      }

      /// Returns the number of nodes composing the BST.
      ///
      /// Complexity O(n).
      ///
      /// @return number of nodes composing the tree.
      std::size_t Size() const
      {
        return 1 + ((this->leftChild) ? this->leftChild->Size() : 0)
                 + ((this->rightChild) ? this->rightChild->Size() : 0);
      }

      Value GetData() const { return this->data; }
      const BST* GetLeftChild() const { return this->leftChild.get(); }
      const BST* GetRightChild() const { return this->rightChild.get(); }

    private:
      BST(const Value& data) : data(data) {}
      BST(BST&) {}           // Not Implemented
      BST operator=(BST&) {} // Not Implemented

      /// Check validity of the Binary Search Tree.
      /// Recursively check if subtrees do not violate any of the rules defined by a BST.
      ///
      /// @param previousNode unique_ptr reference on const BST* used to keep track of the last node retrieved.
      ///
      /// @remark Using a preordering traversal, it makes sure that:
      /// - If the node is the left child of its parent, then it must be smaller than (or equal to)
      ///   the parent and it must pass down the value from its parent to its right subtree to make sure none
      ///   of the nodes in that subtree is greater than the parent.
      /// - If the node is the right child of its parent, then it must be larger than the parent and it must
      ///   pass down the value from its parent to its left subtree to make sure none of the nodes in that
      ///   subtree is lesser than the parent.
      ///
      /// @return wheter or not the tree is a valid Binary Search Tree (true) or not (false).
      bool IsValid(std::unique_ptr<const BST*>& previousNode) const
      {
        // Recurse on left child without breaking if not failing
        if (this->leftChild &&!this->leftChild->IsValid(previousNode))
          return false;

        // First node retrieve - assign
        if (!*previousNode)
          *previousNode = this;
        // Previous data does not compare well to the current one - BST not valid
        else if (!Compare()((*previousNode)->data, this->data))
          return false;

        // Set current node
        *previousNode = this;

        // Recurse on right child
        if (this->rightChild && !this->rightChild->IsValid(previousNode))
          return false;

        return true;
      }

      /// Retrieve the Predecessor unique_ptr reference.
      ///
      /// @warning this method should not be called if no left child exists  [assert].
      ///
      /// @return the predecessor unique_ptr reference.
      std::unique_ptr<BST>& GetPredecessor()
      {
        // Cannot get predecessor if no left child exists
        assert(("GetPredecessor should not be called if no left child exists.", this->leftChild));

        // Left child is the predecessor
        if (!this->leftChild->rightChild)
          return this->leftChild;

        // Return Right Most Child
        return this->leftChild->GetRightMostChild();
      }

      /// Retrieve the Right Most Child unique_ptr reference.
      ///
      /// @warning this method should not be called if no right child exists [assert].
      ///
      /// @return the right most child unique_ptr reference.
      std::unique_ptr<BST>& GetRightMostChild()
      {
        // Cannot get predecessor if no left child exists
        assert(("GetRightMostChild should not be called if no right child exists.", this->rightChild));

        if (this->rightChild && !this->rightChild->rightChild)
          return this->rightChild;

        return this->rightChild->GetRightMostChild();
      }

      void SetLeftChild(std::unique_ptr<BST> bst) { this->leftChild = std::move(bst); }
      void SetRightChild(std::unique_ptr<BST> bst) { this->rightChild = std::move(bst); }

      typename std::iterator_traits<IT>::value_type data;
      std::unique_ptr<BST> leftChild;
      std::unique_ptr<BST> rightChild;
  };
};

#endif // MODULE_DATA_STRUCTURES_BST_HXX
