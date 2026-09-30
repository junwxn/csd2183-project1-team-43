// Your red-black tree. Every member function below is a stub that compiles
// but does nothing useful yet; replace each one with your implementation.

#include "red_black_tree.h"

// TODO: Add the members your implementation needs, such as the key, the
// value, the color, and links to the children and the parent.
struct RBNode {};

RedBlackTree::RedBlackTree() {
  // TODO: Set up the tree, for example a sentinel for the NIL leaves.

  // These two lines only silence compiler warnings about unused members.
  // Delete them once your code uses root_ and nil_.
  (void)root_;
  (void)nil_;
}

RedBlackTree::~RedBlackTree() {
  // TODO: Release all nodes.
}

bool RedBlackTree::insert(std::int64_t /*key*/, std::int64_t /*value*/) {
  return false;  // TODO
}

bool RedBlackTree::erase(std::int64_t /*key*/) {
  return false;  // TODO
}

std::optional<std::int64_t> RedBlackTree::find(std::int64_t /*key*/) const {
  return std::nullopt;  // TODO
}

std::size_t RedBlackTree::size() const {
  return size_;
}

std::vector<std::int64_t> RedBlackTree::keys() const {
  return {};  // TODO
}

void RedBlackTree::export_shape(std::ostream & /*out*/) const {
  // TODO
}

int RedBlackTree::height() const {
  return -1;  // TODO
}

double RedBlackTree::average_depth() const {
  return 0.0;  // TODO
}
