#!/bin/bash
cd "$(dirname "$0")/../.." || exit 2
fails=0
 
. scripts/selftest/check_x.sh
echo "=== Valid Controls ==="

check_x "rb_valid_empty" "rb" "PASS" << 'EOF'
root -
EOF

check_x "rb_valid_single" "rb" "PASS" << 'EOF'
root 5
5 B - -
EOF

check_x "rb_valid_mixed" "rb" "PASS" << 'EOF'
root 5
3 R - -
5 B 3 8
8 R - -
EOF

check_x "rb_valid_deep" "rb" "PASS" << 'EOF'
root 10
3 B - -
5 R 3 8
8 B - -
10 B 5 20
20 B - 25
25 R - -
EOF

check_x "avl_valid_empty" "avl" "PASS" << 'EOF'
root -
EOF

check_x "avl_valid_single" "avl" "PASS" << 'EOF'
root 5
5 0 - -
EOF

check_x "avl_valid_multi" "avl" "PASS" << 'EOF'
root 5
3 0 - -
5 1 3 8
7 0 - -
8 -1 7 -
EOF

check_x "avl_valid_tall" "avl" "PASS" << 'EOF'
root 10
3 0 - -
5 -1 3 -
10 1 5 15
12 0 - -
15 1 12 20
20 1 - 25
25 0 - -
EOF

echo "=== Traversal & Structural Rules ==="

check_x "dangling_child" "rb" "dangling child 3" << 'EOF'
root 5
5 B 3 -
EOF

check_x "shared_child" "avl" "cycle or shared child at 3" << 'EOF'
root 5
3 0 - -
5 0 3 8
8 0 3 -
EOF

check_x "cycle" "avl" "cycle or shared child at 5" << 'EOF'
root 5
5 0 - 8
8 0 5 -
EOF

check_x "unreachable_node" "rb" "unreachable nodes detected" << 'EOF'
root 5
5 B - -
8 B - -
EOF

check_x "bst_violation_deep" "rb" "BST violation at 15" << 'EOF'
root 10
5 B - 15
10 B 5 -
15 R - -
EOF

check_x "bst_violation_deep_right" "rb" "BST violation at 5" << 'EOF'
root 10
5 R - -
10 B - 15
15 R 5 -
EOF

check_x "unreachable_cycle" "avl" "unreachable nodes detected" << 'EOF'
root 5
5 0 - -
8 0 9 -
9 0 8 -
EOF

check_x "dangling_child_deep" "rb" "dangling child 12" << 'EOF'
root 10
5 B - -
10 B 5 15
15 R - 12
EOF

echo "=== Red-Black Rules ==="

check_x "rb_red_root" "rb" "root 5 is not black" << 'EOF'
root 5
5 R - -
EOF

check_x "rb_red_red" "rb" "red node 5 has red child 8" << 'EOF'
root 10
5 R - 8
8 R - -
10 B 5 -
EOF

check_x "rb_unequal_black" "rb" "node 10: left black-height 2, right black-height 1" << 'EOF'
root 10
5 B - -
10 B 5 -
EOF

check_x "rb_red_red_left" "rb" "red node 10 has red child 5" << 'EOF'
root 15
5 R - -
10 R 5 -
15 B 10 -
EOF

check_x "rb_unequal_deep" "rb" "node 15: left black-height 2, right black-height 1" << 'EOF'
root 10
5 B - -
10 B 5 15
12 B - -
15 B 12 -
EOF

check_x "rb_unequal_red_involved" "rb" "node 10: left black-height 1, right black-height 2" << 'EOF'
root 10
5 R - -
10 B 5 15
15 B - -
EOF

check_x "rb_combined_same_node" "rb" "node 10: left black-height 1, right black-height 2" << 'EOF'
root 20
5 R - -
10 R 5 15
15 B - -
20 B 10 -
EOF

echo "=== AVL Rules ==="

check_x "avl_wrong_factor" "avl" "node 5: stated factor 0, true factor 1" << 'EOF'
root 5
5 0 - 9
9 0 - -
EOF

check_x "avl_unbalanced" "avl" "node 5: stated factor 0, true factor -2 (unbalanced)" << 'EOF'
root 5
1 0 - -
3 -1 1 -
5 0 3 -
EOF

check_x "avl_wrong_factor_deep" "avl" "node 15: stated factor 1, true factor -1" << 'EOF'
root 10
5 0 - -
10 1 5 15
12 0 - -
15 1 12 -
EOF

check_x "avl_wrong_sign" "avl" "node 5: stated factor -1, true factor 1" << 'EOF'
root 5
5 -1 - 9
9 0 - -
EOF

check_x "avl_unbalanced_right" "avl" "node 5: stated factor 0, true factor 2 (unbalanced)" << 'EOF'
root 5
5 0 - 8
8 1 - 9
9 0 - -
EOF

check_x "avl_wrong_root_only" "avl" "node 5: stated factor -1, true factor 0" << 'EOF'
root 5
3 0 - -
5 -1 3 8
8 0 - -
EOF

echo "$fails miss(es)"
[ "$fails" -eq 0 ]