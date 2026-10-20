#include "red_black_tree.h"
#include <algorithm>
#include <utility>

enum class Colour : bool {
  Red,  
  Black
};

struct RBNode {
  std::int64_t key{};
  std::int64_t value{};
  Colour colour = Colour::Black; // default black (for sentinels)
  RBNode *left = nullptr, *right = nullptr, *parent = nullptr;
};

//helper functions
namespace{

  void left_rotate(RBNode *&root, RBNode *nil, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != nil) y->left->parent = x;
    y->parent = x->parent;


    if (x->parent == nil) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void right_rotate(RBNode *&root, RBNode *nil, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nil) root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;

    x->right = y;
    y->parent = x;
  }

  void insert_fixup(RBNode *&root, RBNode *nil, RBNode *z) {
    while (z->parent->colour == Colour::Red) {
      if (z->parent == z->parent->parent->left) {
        RBNode *uncle = z->parent->parent->right;
        //case 1 (red uncle): recolour and move z upwards
        if (uncle->colour == Colour::Red) {
          z->parent->colour = Colour::Black;
          uncle->colour = Colour::Black;
          z->parent->parent->colour = Colour::Red;
          z = z->parent->parent;
        } else {
          //case 2 (black uncle, z is right child): left rotate
          if (z == z->parent->right) {
            z = z->parent;
            left_rotate(root, nil, z);
          }
          //case 3(black uncle, z is left child): right rotate & recolour
          z->parent->colour = Colour::Black;
          z->parent->parent->colour = Colour::Red;
          right_rotate(root, nil, z->parent->parent);

        }
        //mirror cases for 1,2,3
      } else {
        RBNode *uncle = z->parent->parent->left;
        if (uncle->colour == Colour::Red) {
          z->parent->colour = Colour::Black;
          uncle->colour = Colour::Black;
          z->parent->parent->colour = Colour::Red;
          z = z->parent->parent;
        } else {
          if (z == z->parent->left) {
            z = z->parent;
            right_rotate(root, nil, z);
          }
          z->parent->colour = Colour::Black;
          z->parent->parent->colour = Colour::Red;
          left_rotate(root, nil, z->parent->parent);
        }

      }
    }
    root->colour = Colour::Black;
  }

  RBNode *search(RBNode *root, RBNode *nil, std::int64_t key) {
    while (root != nil && root->key != key)
      root = key < root->key ? root->left : root->right;

    return root;
  }

  void transplant(RBNode *&root, RBNode *nil, RBNode *u, RBNode *v) {
    if (u->parent == nil) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;

    v->parent = u->parent;  // part of clrs
  }

  RBNode *minimum(RBNode *x, RBNode *nil) {
    while (x->left != nil) x = x->left;
    return x;
  }

  void delete_fixup(RBNode *&root, RBNode *nil, RBNode *x) {
    while (x != root && x->colour == Colour::Black) {
      if (x == x->parent->left) {
        RBNode *w = x->parent->right;
        // case 1 (red sibling): recolour and rotate
        if (w->colour == Colour::Red) {
          w->colour = Colour::Black;
          x->parent->colour = Colour::Red;
          left_rotate(root, nil, x->parent);
          w = x->parent->right;

        }
        //case 2 (sibing and its children ALL black)
        if (w->left->colour == Colour::Black &&
            w->right->colour == Colour::Black) {
          w->colour = Colour::Red;

          x = x->parent;
        } else {
          //case 3 (sibling is black and its left child red + right child black)
          if (w->right->colour == Colour::Black) {
            w->left->colour = Colour::Black;
            w->colour = Colour::Red;

            right_rotate(root, nil, w);
            w = x->parent->right;
          }
          //case 4 (sibling is black and its right child is red)
          w->colour = x->parent->colour;
          x->parent->colour = Colour::Black;
          w->right->colour =  Colour::Black;

          left_rotate(root, nil, x->parent);
          x = root;
        }
      } else { // mirror cases
        RBNode *w = x->parent->left;
        if (w->colour == Colour::Red) {
          w->colour = Colour::Black;

          x->parent->colour = Colour::Red;
          right_rotate(root, nil, x->parent);
          w = x->parent->left;
        }
        if (w->right->colour == Colour::Black &&
            w->left->colour == Colour::Black) {
          w->colour = Colour::Red;
          x = x->parent;
        } else {
          if (w->left->colour == Colour::Black) {
            w->right->colour = Colour::Black;
            w->colour = Colour::Red;
            left_rotate(root, nil, w);
            w = x->parent->left;
          }


          w->colour = x->parent->colour;
          x->parent->colour = Colour::Black;
          w->left->colour = Colour::Black;
          right_rotate(root, nil, x->parent);
          x = root;
        }
      }
    }
    x->colour = Colour::Black;  // absorb any remaining extra black.
  }

  void destroy(RBNode *x, RBNode *nil) {
    if (x == nil) return;

    destroy(x->left, nil);
    destroy(x->right, nil);
    delete x;
  }

  //in order traversal
  void collect_keys(RBNode *x, RBNode *nil, std::vector<std::int64_t> &out) {
    if (x == nil) return;

    collect_keys(x->left, nil, out);
    out.push_back(x->key);
    collect_keys(x->right, nil, out);
  }

  //track max depth and sum of depth
  void depth_stats(RBNode *x, RBNode *nil, int depth, int &max_depth, double &sum_depth) {
    if (x == nil) return;

    max_depth = std::max(max_depth, depth);
    sum_depth += depth;

    depth_stats(x->left, nil, depth + 1, max_depth, sum_depth);
    depth_stats(x->right, nil, depth + 1, max_depth, sum_depth);
  }

} //end namespace



RedBlackTree::RedBlackTree() { // default ctor
  nil_ = new RBNode; //default colour is black (sentinel nodes)
  nil_->parent = nil_->left = nil_->right = nil_;
  root_ = nil_;
}

RedBlackTree::~RedBlackTree() { // dtor
  destroy(root_, nil_);
  delete nil_;
}

bool RedBlackTree::insert(std::int64_t key, std::int64_t value) {
  RBNode *parent = nil_;
  RBNode *x = root_;
  while (x != nil_) {
    parent = x;
    if (key == x->key) return false;
    x = key < x->key ? x->left : x->right;
  }
  // new node start red
  RBNode *z = new RBNode{key, value, Colour::Red, nil_, nil_, parent};

  if (parent == nil_) root_ = z;
  else if (key < parent->key) parent->left = z;
  else parent->right = z;
  
  insert_fixup(root_, nil_, z);
  
  ++size_;
  return true;
}

bool RedBlackTree::erase(std::int64_t key) {
  RBNode *z = search(root_, nil_, key);
  if (z == nil_) return false;
  RBNode *y = z;
  Colour original = y->colour;
  RBNode *x = nil_;
  if (z->left == nil_) {
    x = z->right;
    transplant(root_, nil_, z, z->right);
  } else if (z->right == nil_) {
    x = z->left;
    transplant(root_, nil_, z, z->left);
  } else {
    y = minimum(z->right, nil_);
    original = y->colour;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;  
    } else {
      transplant(root_, nil_, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(root_, nil_, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->colour = z->colour;
  }
  delete z;  
  --size_;
  if (original == Colour::Black) delete_fixup(root_, nil_, x);
  return true;
}


std::optional<std::int64_t> RedBlackTree::find(std::int64_t key) const {
  RBNode *x = search(root_, nil_, key);
  if (x == nil_) return std::nullopt;
  return x->value;
}


std::size_t RedBlackTree::size() const {
  return size_;
}

std::vector<std::int64_t> RedBlackTree::keys() const {
  std::vector<std::int64_t> result;
  result.reserve(size_);
  collect_keys(root_, nil_, result);
  return result;
}

void RedBlackTree::export_shape(std::ostream &out) const {
    // print the root key
    if (root_ == nil_) {
        out << "root -\n";
        return;
    }

    out << "root " << root_->key << '\n';

    //recursive lambda traverse 
    auto traverse = [&](auto&& self, RBNode* node) -> void {

        // return when reach the end
        if (node == nil_) {
            return;
        }

        // find smalles key first
        self(self, node->left);

        // print key followed by colour
        out << node->key << ' ' << (node->colour == Colour::Black ? 'B' : 'R') << ' ';

        //print left child
        if (node->left == nil_) {
            out << "-";
        } else {
            out << node->left->key;
        }

        out << ' ';

        //print right child
        if (node->right == nil_) {
            out << "-";
        } else {
            out << node->right->key;
        }

        out << '\n';

        //move right
        self(self, node->right);
    };

    traverse(traverse, root_);
}

int RedBlackTree::height() const {
  int max_depth = -1;
  double sum_depth = 0;
  depth_stats(root_, nil_, 0, max_depth, sum_depth);
  return max_depth;
}

double RedBlackTree::average_depth() const {
  if (size_ == 0) return 0.0;
  int max_depth = -1;
  double sum_depth = 0;
  depth_stats(root_, nil_, 0, max_depth, sum_depth);
  return sum_depth / static_cast<double>(size_);
}
