#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <string>

#include "avl_tree.h"
#include "red_black_tree.h"

bool run_experiments(int64_t n) {
    bool passed = true;
    double ideal_bound = std::log2(n);
    double avl_max_bound = 1.44 * std::log2(n + 2.0);
    double rb_max_bound = 2.0 * std::log2(n + 1.0);

    std::vector<int64_t> keys(n);
    std::iota(keys.begin(), keys.end(), 1);

    // Helper to log errors to stderr so stdout (CSV) remains uncorrupted
    auto check_and_log = [&](int h, double max_bound, const std::string& tree, const std::string& order, int trial) {
        if (h < std::floor(ideal_bound) || h > max_bound) {
            std::cerr << "Bound Error: N=" << n << ", Order=" << order << ", Trial=" << trial
                << ", Tree=" << tree << ", Height=" << h << ", MaxBound=" << max_bound << "\n";
            return false;
        }
        return true;
        };

    // 1. Sorted Insertion (Ascending)
    AVLTree avl_asc; RedBlackTree rbt_asc;
    for (int64_t key : keys) {
        avl_asc.insert(key, key * 10);
        rbt_asc.insert(key, key * 10);
    }

    int ah_asc = avl_asc.height(); double ad_asc = avl_asc.average_depth();
    int rh_asc = rbt_asc.height(); double rd_asc = rbt_asc.average_depth();

    // Print CSV rows first
    std::cout << "AVL," << n << ",Sorted,0,0," << ah_asc << "," << ad_asc << "\n";
    std::cout << "RB," << n << ",Sorted,0,0," << rh_asc << "," << rd_asc << "\n";

    // Then check bounds and integrity
    passed &= check_and_log(ah_asc, avl_max_bound, "AVL", "Sorted", 0);
    passed &= check_and_log(rh_asc, rb_max_bound, "RB", "Sorted", 0);

    for (int64_t key : keys) {
        // find() returns std::optional<int64_t>; this fails if the key is missing or the value is wrong
        if (avl_asc.find(key) != key * 10) { std::cerr << "AVL Sorted dropped key " << key << "\n"; passed = false; break; }
        if (rbt_asc.find(key) != key * 10) { std::cerr << "RB Sorted dropped key " << key << "\n"; passed = false; break; }
    }

    if (avl_asc.size() != static_cast<std::size_t>(n) || rbt_asc.size() != static_cast<std::size_t>(n)) {
        std::cerr << "Size mismatch (Sorted): N=" << n << "\n";
        passed = false;
    }

    // 2. Random Insertions (10 Trials)
    bool shuffle_changed = false; // stays false if every random trial matches the sorted run
    for (int i = 1; i <= 10; ++i) {
        uint32_t seed = 42 + i; // Explicit per-trial seed
        std::mt19937 gen(seed);
        std::vector<int64_t> rand_keys = keys;

        // Note: std::shuffle permutations depend on the compiler's standard library implementation.
        // M4 must use the exact same build environment if identical keys are required.
        std::shuffle(rand_keys.begin(), rand_keys.end(), gen);

        AVLTree avl; RedBlackTree rbt;
        for (int64_t key : rand_keys) {
            avl.insert(key, key * 10);
            rbt.insert(key, key * 10);
        }

        int ah = avl.height(); double ad = avl.average_depth();
        int rh = rbt.height(); double rd = rbt.average_depth();

        std::cout << "AVL," << n << ",Random," << i << "," << seed << "," << ah << "," << ad << "\n";
        std::cout << "RB," << n << ",Random," << i << "," << seed << "," << rh << "," << rd << "\n";

        passed &= check_and_log(ah, avl_max_bound, "AVL", "Random", i);
        passed &= check_and_log(rh, rb_max_bound, "RB", "Random", i);

        if (ad != ad_asc || rd != rd_asc) shuffle_changed = true;

        // Check all keys are still present
        for (int64_t key : keys) {
            if (avl.find(key) != key * 10) { std::cerr << "AVL Random dropped key " << key << "\n"; passed = false; break; }
            if (rbt.find(key) != key * 10) { std::cerr << "RB Random dropped key " << key << "\n"; passed = false; break; }
        }

        if (avl.size() != static_cast<std::size_t>(n) || rbt.size() != static_cast<std::size_t>(n)) {
            std::cerr << "Size mismatch (Random, Trial=" << i << "): N=" << n << "\n";
            passed = false;
        }
    }

    if (!shuffle_changed) {
        std::cerr << "Shuffle Error: N=" << n << ", all random trials identical to sorted run\n";
        passed = false;
    }
    return passed;
}

int main() {
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Tree,N,Order,Trial,Seed,Height,AvgDepth\n";

    // M4 specific sizes (Ensure these are explicitly confirmed with M4)
    std::vector<int64_t> sizes = { 4096, 16384, 65536, 262144 };

    bool all_passed = true;
    for (int64_t n : sizes) {
        if (!run_experiments(n)) {
            all_passed = false;
        }
    }

    // Exit non-zero if any tree violated its bounds or dropped keys
    return all_passed ? 0 : 1;
}