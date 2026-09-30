// Your AVL tree. Every member function below is a stub that compiles but does
// nothing useful yet; replace each one with your implementation.

#include "avl_tree.h"

// TODO: Add the members your implementation needs, such as the key, the
// value, the height or balance factor, and links to the children.
struct AVLNode {};

AVLTree::AVLTree() {
  // TODO: Set up the tree, if anything is needed beyond an empty root.

  // This line only silences a compiler warning about an unused member.
  // Delete it once your code uses root_.
  (void)root_;
}

AVLTree::~AVLTree() {
  // TODO: Release all nodes.
}

bool AVLTree::insert(std::int64_t /*key*/, std::int64_t /*value*/) {
  return false;  // TODO
}

bool AVLTree::erase(std::int64_t /*key*/) {
  return false;  // TODO
}

std::optional<std::int64_t> AVLTree::find(std::int64_t /*key*/) const {
  return std::nullopt;  // TODO
}

std::size_t AVLTree::size() const {
  return size_;
}

std::vector<std::int64_t> AVLTree::keys() const {
  return {};  // TODO
}

void AVLTree::export_shape(std::ostream & /*out*/) const {
  // TODO
}

int AVLTree::height() const {
  return -1;  // TODO
}

double AVLTree::average_depth() const {
  return 0.0;  // TODO
}
