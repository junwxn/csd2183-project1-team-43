// DO NOT MODIFY THIS FILE. Gradescope replaces it with the original before
// building your submission.

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <ostream>
#include <vector>

// A node of the AVL tree. Define this struct, with the members your
// implementation needs, in a source or header file of your own in src/.
struct AVLNode;

// An AVL tree that maps unique keys to values.
class AVLTree {
 public:
  AVLTree();

  // Releases all nodes.
  ~AVLTree();

  AVLTree(const AVLTree &) = delete;
  AVLTree &operator=(const AVLTree &) = delete;

  // Inserts the pair unless the key is already present, in which case the
  // tree is left unchanged. Returns true if the pair was inserted.
  bool insert(std::int64_t key, std::int64_t value);

  // Deletes the key if it is present. Returns true if a key was deleted.
  bool erase(std::int64_t key);

  // Returns the value stored with the key, or std::nullopt if the key is
  // absent.
  std::optional<std::int64_t> find(std::int64_t key) const;

  // Returns the number of keys in the tree.
  std::size_t size() const;

  // Returns all keys in ascending order.
  std::vector<std::int64_t> keys() const;

  // Writes the output of the X command, as specified in the assignment
  // brief, to out.
  void export_shape(std::ostream &out) const;

  // Returns the largest depth of any node, where the root has depth 0, or -1
  // for an empty tree.
  int height() const;

  // Returns the mean depth of all nodes, where the root has depth 0, or 0.0
  // for an empty tree.
  double average_depth() const;

 private:
  AVLNode *root_ = nullptr;
  std::size_t size_ = 0;
};

#endif  // AVL_TREE_H
