// An example benchmark with nanobench (https://nanobench.ankerl.com).
//
// Build and run it with
//
//   make bench
//   build/example        (build\example.exe in PowerShell)
//
// It shows the three patterns you will need, each after sorted and after
// random insertion. The first times a short operation, find(), on a structure
// that stays alive across measurements. The
// second times a bulk operation, inserting n keys, for which every measurement
// must start from an empty structure that is created, and later destroyed,
// outside the timed code. The third times deleting all n keys, for which every
// measurement must start from a full structure that is built outside the timed
// code. All three use MapBaseline; once your trees work,
// uncomment the calls in main() to time them under the same conditions. Copy
// this file to write your own benchmarks: every .cpp file in bench/ becomes a
// separate program in build/. Do not define ANKERL_NANOBENCH_IMPLEMENT; the
// Makefile already links the library into every benchmark program.

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "avl_tree.h"
#include "map_baseline.h"
#include "nanobench.h"
#include "red_black_tree.h"

namespace {

    // The sizes of all three benchmarks: four sizes, the largest 2^18 keys as in
    // Brown (2025). Add sizes as your experiments need.
    constexpr std::size_t kSizes[] = { std::size_t{1} << 12,
                                       std::size_t{1} << 14,
                                       std::size_t{1} << 16,
                                       std::size_t{1} << 18 
    };

    // Times find() on a structure of type Tree that holds the given keys,
    // inserted in the given order, and adds the result to bench.
    template <typename Tree>
    void benchmark_find(ankerl::nanobench::Bench& bench, const std::string& name,
        const std::vector<std::int64_t>& insert_keys) {
        Tree tree;
        for (const std::int64_t key : insert_keys) {
            tree.insert(key, key);
        }

        // Search the keys in a random order, independent of the insertion order.
        std::vector<std::int64_t> search_keys = insert_keys;
        std::mt19937_64 rng(2183);
        std::shuffle(search_keys.begin(), search_keys.end(), rng);
        std::size_t i = 0;
        bench.run(name + " find, n = " + std::to_string(search_keys.size()), [&] {
            ankerl::nanobench::doNotOptimizeAway(tree.find(search_keys[i]));
            i = (i + 1 == search_keys.size()) ? 0 : i + 1;
            });
    }

    // Times the insertion of all keys, in the given order, into an empty structure
    // of type Tree, and adds the result to bench. nanobench times everything inside
    // run(), so the empty structures are created beforehand, one for each
    // measurement, and destroyed only after run() returns; releasing the nodes is
    // not part of the insertion time. bench must be set to exactly one iteration
    // per epoch, as in main().
    template <typename Tree>
    void benchmark_insert(ankerl::nanobench::Bench& bench, const std::string& name,
        const std::vector<std::int64_t>& keys) {
        std::vector<std::unique_ptr<Tree>> trees;
        for (std::size_t i = 0; i < bench.epochs(); ++i) {
            trees.push_back(std::make_unique<Tree>());
        }

        // at() throws if nanobench ever needs more structures than were created.
        std::size_t next = 0;
        bench.batch(keys.size())
            .run(name + " insert, n = " + std::to_string(keys.size()), [&] {
            Tree& tree = *trees.at(next++);
            for (const std::int64_t key : keys) {
                tree.insert(key, key);
            }
            ankerl::nanobench::doNotOptimizeAway(tree.size());
                });
    }

    // Times the deletion of all keys, in the order erase_keys, from a structure of
    // type Tree that holds exactly these keys, inserted in the order insert_keys,
    // and adds the result to bench. nanobench times everything inside run(), so
    // the full structures are built beforehand, one for each measurement: bench
    // must be set to exactly one iteration per epoch, as in main().
    template <typename Tree>
    void benchmark_erase(ankerl::nanobench::Bench& bench, const std::string& name,
        const std::vector<std::int64_t>& insert_keys,
        const std::vector<std::int64_t>& erase_keys) {
        std::vector<std::unique_ptr<Tree>> trees;
        for (std::size_t i = 0; i < bench.epochs(); ++i) {
            trees.push_back(std::make_unique<Tree>());
            for (const std::int64_t key : insert_keys) {
                trees.back()->insert(key, key);
            }
        }

        // at() throws if nanobench ever needs more structures than were built.
        std::size_t next = 0;
        bench.batch(erase_keys.size())
            .run(name + " erase, n = " + std::to_string(erase_keys.size()), [&] {
            Tree& tree = *trees.at(next++);
            for (const std::int64_t key : erase_keys) {
                tree.erase(key);
            }
            ankerl::nanobench::doNotOptimizeAway(tree.size());
                });
    }

}  // namespace

int main() {
    // Short operations need many iterations per measurement for stable results;
    // nanobench prints a warning when a measurement is unstable.
    ankerl::nanobench::Bench find_bench;
    find_bench.title("Search for existing keys")
        .unit("find")
        .warmup(1'000)
        .minEpochIterations(200'000);

    std::mt19937_64 rng(2183);
    for (const std::size_t n : kSizes) {
        std::vector<std::int64_t> sorted_keys(n);
        std::iota(sorted_keys.begin(), sorted_keys.end(), std::int64_t{ 0 });
        std::vector<std::int64_t> random_keys = sorted_keys;
        std::shuffle(random_keys.begin(), random_keys.end(), rng);

        benchmark_find<MapBaseline>(find_bench, "map, sorted", sorted_keys);
        benchmark_find<MapBaseline>(find_bench, "map, random", random_keys);
        benchmark_find<RedBlackTree>(find_bench, "rb, sorted", sorted_keys);
        benchmark_find<RedBlackTree>(find_bench, "rb, random", random_keys);
        benchmark_find<AVLTree>(find_bench, "avl, sorted", sorted_keys);
        benchmark_find<AVLTree>(find_bench, "avl, random", random_keys);
    }

    // Each insertion measurement fills one empty structure, so every epoch
    // consists of exactly one iteration, and nanobench reports the median over
    // the epochs, as for deletion below. batch() in benchmark_insert reports the
    // time per inserted key rather than per bulk insertion.
    ankerl::nanobench::Bench insert_bench;
    insert_bench.title("Insert n keys into an empty structure")
        .unit("insert")
        .epochs(11)
        .epochIterations(1);

    for (const std::size_t n : kSizes) {
        std::vector<std::int64_t> sorted_keys(n);
        std::iota(sorted_keys.begin(), sorted_keys.end(), std::int64_t{ 0 });
        std::vector<std::int64_t> random_keys = sorted_keys;
        std::shuffle(random_keys.begin(), random_keys.end(), rng);

        benchmark_insert<MapBaseline>(insert_bench, "map, sorted", sorted_keys);
        benchmark_insert<MapBaseline>(insert_bench, "map, random", random_keys);
        benchmark_insert<RedBlackTree>(insert_bench, "rb, sorted", sorted_keys);
        benchmark_insert<RedBlackTree>(insert_bench, "rb, random", random_keys);
        benchmark_insert<AVLTree>(insert_bench, "avl, sorted", sorted_keys);
        benchmark_insert<AVLTree>(insert_bench, "avl, random", random_keys);
    }

    // Each deletion measurement consumes one full structure, so every epoch
    // consists of exactly one iteration, and nanobench reports the median over
    // the epochs. For small n, one iteration takes only microseconds, and
    // nanobench may mark the result as unstable; its advice to increase
    // minEpochIterations has no effect here, so treat such results with caution
    // and rely on larger n.
    ankerl::nanobench::Bench erase_bench;
    erase_bench.title("Delete all n keys from a full structure")
        .unit("erase")
        .epochs(11)
        .epochIterations(1);

    for (const std::size_t n : kSizes) {
        std::vector<std::int64_t> sorted_keys(n);
        std::iota(sorted_keys.begin(), sorted_keys.end(), std::int64_t{ 0 });
        std::vector<std::int64_t> insert_keys = sorted_keys;
        std::shuffle(insert_keys.begin(), insert_keys.end(), rng);
        std::vector<std::int64_t> erase_keys = sorted_keys;
        std::shuffle(erase_keys.begin(), erase_keys.end(), rng);

        benchmark_erase<MapBaseline>(erase_bench, "map, sorted", sorted_keys,
            sorted_keys);
        benchmark_erase<MapBaseline>(erase_bench, "map, random", insert_keys,
            erase_keys);
        benchmark_erase<RedBlackTree>(erase_bench, "rb, sorted", sorted_keys,
                                      sorted_keys);
        benchmark_erase<RedBlackTree>(erase_bench, "rb, random", insert_keys,
                                      erase_keys);
        benchmark_erase<AVLTree>(erase_bench, "avl, sorted", sorted_keys,
                                 sorted_keys);
        benchmark_erase<AVLTree>(erase_bench, "avl, random", insert_keys,
                                 erase_keys);
    }
}
