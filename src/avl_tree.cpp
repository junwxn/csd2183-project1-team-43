// Your AVL tree. Every member function below is a stub that compiles but does
// nothing useful yet; replace each one with your implementation.

#include "avl_tree.h"

// TODO: Add the members your implementation needs, such as the key, the
// value, the height or balance factor, and links to the children.
struct AVLNode {
  std::int64_t key{};
  std::int64_t value{};
  int height{};
  AVLNode *left = nullptr, *right = nullptr, *parent = nullptr;
};

// Helper functions
namespace {
  // Helper function destroy() that recursively releases all pointers and 
  // effectively "deletes" the AVL tree.
  void destroy(AVLNode *node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

  // Helper function get_height to retreive the height of the current node
  int get_height(AVLNode *curr) {
    return curr ? curr->height : -1;
  }

  // Helper function update_height to update the height of the node based on
  // the height of their child nodes
  void update_height(AVLNode *curr) {
    if (get_height(curr->left) > get_height(curr->right)) {
      curr->height = 1 + get_height(curr->left);
    } else {
      curr->height = 1 + get_height(curr->right);
    }
  }

  // Helper function balance_factor() that returns the balance factor
  int balance_factor(AVLNode *curr) {
    return get_height(curr->right) - get_height(curr->left);
  }

  // Helper function find_min() to find the min of a particular node downwards
  AVLNode *find_min(AVLNode *curr) {
    if(curr == nullptr) return nullptr;
    while (curr->left != nullptr) {
      curr = curr->left;
    }
    return curr;
  }

  // Helper function left_rotate() that rotates the nodes to the left
  AVLNode *left_rotate(AVLNode *curr) {
    AVLNode *new_root = curr->right;
    curr->right = new_root->left;
    new_root->left = curr;

    update_height(curr);
    update_height(new_root);
    return new_root;
  }

  // Helper function right_rotate() that rotates the nodes to the right
  AVLNode *right_rotate(AVLNode *curr) {
    AVLNode *new_root = curr->left;
    curr->left = new_root->right;
    new_root->right = curr;

    update_height(curr);
    update_height(new_root);
    return new_root;
  }

  // Helper function rebalance() which will rebalance the node of the AVL tree 
  AVLNode *rebalance(AVLNode *curr) {
    update_height(curr);
    int balance = balance_factor(curr);

    // left tree higher
    if (balance < -1) {
      // left child leans right
      if (balance_factor(curr->left) > 0) {
        curr->left = left_rotate(curr->left);
      }
      return right_rotate(curr);
    }
    // right tree higher
    if (balance > 1) {
      // right child leans left
      if (balance_factor(curr->right) < 0) {
        curr->right = right_rotate(curr->right);
      }
      return left_rotate(curr);
    }

    return curr;
  }

  // Helper function insert_node() that recursively goes down the avl tree to search
  // for the nullpointer that the new node should replace before going back up the
  // recursive stack to rebalance the tree and returning the new root at the end.
  AVLNode *insert_node(AVLNode *curr, std::int64_t key, std::int64_t value, bool &inserted) {
    if (curr == nullptr) {
      inserted = true;
      return new AVLNode{key, value};
    }

    if (key < curr->key) {
      curr->left = insert_node(curr->left,key,value,inserted);
    } else if (key > curr->key) {
      curr->right = insert_node(curr->right,key,value,inserted);
    } else {
      return curr;
    }
    return rebalance(curr);
  }

  // Helper function erase_node() that recursively goes down the tree to find the
  // node to be deleted and replacing that node with it's successor
  AVLNode *erase_node(AVLNode *curr, std::int64_t key, bool &erased) {
    if (curr == nullptr) return nullptr;

    if (key < curr->key) {
      curr->left = erase_node(curr->left, key, erased);
    } else if (key > curr->key) {
      curr->right = erase_node(curr->right, key, erased);
    } else {
      if (curr->left == nullptr || curr->right == nullptr) {
        // zero or one child, safe to delete the node and either replace with child
        // or nullptr
        AVLNode *child = curr->left ? curr->left : curr->right;
        delete curr;
        erased = true;
        return child;
      }

      // 2 children, copy the successor's key and value into curr, then
      // delete the successor from the right subtree
      AVLNode *successor = find_min(curr->right);
      curr->key = successor->key;
      curr->value = successor->value;
      curr->right = erase_node(curr->right, successor->key, erased);
    }
    return rebalance(curr);
  }

  // Helper function to collect and collate all the keys into a vector
  // referenced from red_black_tree
  void collect_keys(AVLNode *curr, std::vector<std::int64_t> &out) {
    if (curr == nullptr) return;
    collect_keys(curr->left, out);
    out.push_back(curr->key);
    collect_keys(curr->right, out);
  }

  // Helper function sum_depths() to add up the depths of all nodes in the
  // avltree.
  double sum_depths(AVLNode *curr, int depth) {
    if (curr == nullptr) return 0.0;
    return depth + sum_depths(curr->left, depth + 1) + sum_depths(curr->right, depth + 1);
  }

  // Helper function print_key to write the node's key or - if absent.
  void print_key(std::ostream &out, AVLNode *curr) {
    if (curr == nullptr) {
      out << '-';
    } else {
      out << curr->key;
    }
  }

  // Helper function print_node() to write one line per node from the left branch
  // ordered as <key> <balance> <left> <right>
  void print_node(std::ostream& out, AVLNode *curr) {
    if (curr == nullptr) return;
    print_node(out, curr->left);

    out << curr->key << ' ' << balance_factor(curr) << ' ';
    print_key(out, curr->left);
    out << ' ';
    print_key(out, curr->right);
    out << '\n';

    print_node(out, curr->right);
  }
}

AVLTree::AVLTree() {
  // TODO: Set up the tree, if anything is needed beyond an empty root.
}

AVLTree::~AVLTree() {
  // TODO: Release all nodes.
  destroy(root_);
}

bool AVLTree::insert(std::int64_t input_key, std::int64_t input_value) {
  bool inserted = false;
  root_ = insert_node(root_, input_key, input_value, inserted);
  if (inserted) ++size_;
  return inserted;
}

bool AVLTree::erase(std::int64_t input_key) {
  bool erased = false;
  root_ = erase_node(root_, input_key, erased);
  if (erased) --size_;
  return erased;
}

std::optional<std::int64_t> AVLTree::find(std::int64_t input_key) const {
  AVLNode *pointer = root_;
  while(pointer != nullptr && pointer->key != input_key) {
    if(pointer->key > input_key) {
      pointer = pointer->left;
    } else {
      pointer = pointer->right;
    }
  }
  if(pointer == nullptr) {
    return std::nullopt;
  } else {
    return pointer->value;
  }
}

std::size_t AVLTree::size() const {
  return size_;
}

std::vector<std::int64_t> AVLTree::keys() const {
  std::vector<std::int64_t> result;
  result.reserve(size_);
  collect_keys(root_, result);
  return result;
}

void AVLTree::export_shape(std::ostream &out) const {
  out << "root ";
  print_key(out, root_);
  out << '\n';
  print_node(out, root_);
}

int AVLTree::height() const {
  return get_height(root_);
}

double AVLTree::average_depth() const {
  if (size_ == 0) return 0.0;
  return sum_depths(root_, 0) / static_cast<double>(size_);
}
