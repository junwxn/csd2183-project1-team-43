// Reads a command file and runs its commands on the selected structure:
//
//   ./tree <rb|avl|map> <command_file>
//
// DO NOT MODIFY THIS FILE. Gradescope replaces it with the original before
// building your submission.
//
// This driver parses the command files and writes the output of the S and P
// commands. The output of X comes from export_shape, which you implement for
// both trees.

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "avl_tree.h"
#include "map_baseline.h"
#include "red_black_tree.h"

namespace {

// Splits `line` into the fields between single spaces. An empty field, caused
// by a leading, trailing, or double space, makes the line unparseable.
std::optional<std::vector<std::string_view>> split(std::string_view line) {
  std::vector<std::string_view> fields;
  std::size_t start = 0;
  while (true) {
    const std::size_t end = line.find(' ', start);
    const std::string_view field = line.substr(start, end - start);
    if (field.empty()) {
      return std::nullopt;
    }
    fields.push_back(field);
    if (end == std::string_view::npos) {
      return fields;
    }
    start = end + 1;
  }
}

// Converts `field` to a signed 64-bit integer, or returns std::nullopt if
// `field` is not exactly such an integer.
std::optional<std::int64_t> to_int64(std::string_view field) {
  std::int64_t value = 0;
  const char *first = field.data();
  const char *last = field.data() + field.size();
  const auto [ptr, error] = std::from_chars(first, last, value);
  if (error != std::errc() || ptr != last) {
    return std::nullopt;
  }
  return value;
}

// Executes the command in `line` on `tree` and writes its output to `out`.
// Returns false if the line cannot be parsed.
template <typename Tree>
bool execute(std::string_view line, Tree &tree, std::ostream &out) {
  const auto fields = split(line);
  if (!fields || (*fields)[0].size() != 1) {
    return false;
  }
  const char command = (*fields)[0][0];
  const std::size_t n_fields = fields->size();

  if (command == 'I' && n_fields == 3) {
    const auto key = to_int64((*fields)[1]);
    const auto value = to_int64((*fields)[2]);
    if (!key || !value) {
      return false;
    }
    tree.insert(*key, *value);
    return true;
  }
  if ((command == 'D' || command == 'S') && n_fields == 2) {
    const auto key = to_int64((*fields)[1]);
    if (!key) {
      return false;
    }
    if (command == 'D') {
      tree.erase(*key);
    } else if (const auto value = tree.find(*key)) {
      out << "S " << *key << " found " << *value << '\n';
    } else {
      out << "S " << *key << " absent\n";
    }
    return true;
  }
  if (command == 'P' && n_fields == 1) {
    out << 'P';
    for (const std::int64_t key : tree.keys()) {
      out << ' ' << key;
    }
    out << '\n';
    return true;
  }
  if (command == 'X' && n_fields == 1) {
    if constexpr (std::is_same_v<Tree, MapBaseline>) {
      out << "X unsupported\n";
    } else {
      tree.export_shape(out);
    }
    return true;
  }
  return false;
}

template <typename Tree>
int run(std::istream &in) {
  Tree tree;
  std::string line;
  while (std::getline(in, line)) {
    // Command files edited on Windows may end each line with '\r'.
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (line.empty() || line.front() == '#') {
      continue;
    }
    if (!execute(line, tree, std::cout)) {
      std::cerr << "Cannot parse line: " << line << '\n';
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void print_usage(const char *program) {
  std::cerr << "Usage: " << program << " <rb|avl|map> <command_file>\n";
}

}  // namespace

int main(int argc, char *argv[]) {
  // Speeds up output for command files with up to a million commands.
  std::ios::sync_with_stdio(false);

  if (argc != 3) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }
  const std::string_view selector = argv[1];
  std::ifstream in(argv[2]);
  if (!in) {
    std::cerr << "Cannot open " << argv[2] << '\n';
    return EXIT_FAILURE;
  }

  if (selector == "rb") {
    return run<RedBlackTree>(in);
  }
  if (selector == "avl") {
    return run<AVLTree>(in);
  }
  if (selector == "map") {
    return run<MapBaseline>(in);
  }
  print_usage(argv[0]);
  return EXIT_FAILURE;
}
