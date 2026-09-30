// DO NOT MODIFY THIS FILE. Gradescope replaces it with the original before
// building your submission.

#ifndef MAP_BASELINE_H
#define MAP_BASELINE_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <vector>

// The std::map baseline, run with the map selector. It provides the same
// member functions as RedBlackTree and AVLTree except export_shape, height,
// and average_depth, because std::map does not expose its internal shape.
class MapBaseline {
 public:
  bool insert(std::int64_t key, std::int64_t value) {
    return map_.try_emplace(key, value).second;
  }

  bool erase(std::int64_t key) {
    return map_.erase(key) > 0;
  }

  std::optional<std::int64_t> find(std::int64_t key) const {
    const auto it = map_.find(key);
    if (it == map_.end()) {
      return std::nullopt;
    }
    return it->second;
  }

  std::size_t size() const {
    return map_.size();
  }

  std::vector<std::int64_t> keys() const {
    std::vector<std::int64_t> result;
    result.reserve(map_.size());
    for (const auto &[key, value] : map_) {
      result.push_back(key);
    }
    return result;
  }

 private:
  std::map<std::int64_t, std::int64_t> map_;
};

#endif  // MAP_BASELINE_H
