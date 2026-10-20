#!/bin/bash
fails=0

check_x() {
    name=$1
    kind=$2
    expected_msg=$3
    out=$(cat | timeout 2s python3 scripts/x_checker.py "$kind" 2>&1)
    code=$?
    
    if [ "$code" -ne 0 ] && grep -qF "$expected_msg" <<< "$out"; then
        echo "ok    $name"
    elif [ "$code" -eq 0 ] && [ "$expected_msg" = "PASS" ]; then
        echo "ok    $name"
    else
        echo "MISS  $name (exit=$code, wanted exit 1 and line: $expected_msg)"
        echo "$out"
        fails=$((fails + 1))
    fi
}

check_x "missing_root" "rb" "missing root line" << 'EOF'
EOF

check_x "two_roots" "rb" "multiple root lines" << 'EOF'
root 5
root 5
5 B - -
EOF

check_x "root_3_fields" "rb" "expected 2" << 'EOF'
root 5 extra
EOF

check_x "node_before_root" "rb" "node line appears before root line" << 'EOF'
5 B - -
root 5
EOF

check_x "node_3_fields" "rb" "expected 4" << 'EOF'
root 5
5 B -
EOF

check_x "key_dash" "rb" "node key cannot be '-'" << 'EOF'
root -
- B - -
EOF

check_x "key_leading_zero" "rb" "invalid root key format" << 'EOF'
root 05
05 B - -
EOF

check_x "key_minus_zero" "rb" "invalid root key format" << 'EOF'
root -0
-0 B - -
EOF

check_x "key_overflow" "rb" "invalid root key format or bounds" << 'EOF'
root 9223372036854775808
9223372036854775808 B - -
EOF

check_x "descending_keys" "rb" "node lines not ascending: 8 then 5" << 'EOF'
root 8
8 B 5 -
5 R - -
EOF

check_x "duplicate_key" "rb" "duplicate key 5" << 'EOF'
root 5
5 B - -
5 B - -
EOF

check_x "root_dash_with_nodes" "rb" "root is - but node lines exist" << 'EOF'
root -
5 B - -
EOF

check_x "root_not_in_nodes" "rb" "root key 10 not in nodes" << 'EOF'
root 10
5 B - -
EOF

check_x "same_child_twice" "rb" "has same child 3 on both sides" << 'EOF'
root 5
3 R - -
5 B 3 3
EOF

check_x "self_child" "rb" "lists itself as a child" << 'EOF'
root 5
5 B 5 -
EOF

check_x "blank_line" "rb" "blank line encountered" << 'EOF'
root 5

5 B - -
EOF

check_x "comment_line" "rb" "comments not allowed" << 'EOF'
root 5
# a comment
5 B - -
EOF

check_x "node_key_leading_zero" "rb" "invalid key or child format/bounds" << 'EOF'
root 5
05 B - -
EOF

check_x "node_key_minus_zero" "rb" "invalid key or child format/bounds" << 'EOF'
root 5
-0 B - -
EOF

check_x "node_key_overflow" "rb" "invalid key or child format/bounds" << 'EOF'
root 5
9223372036854775808 B - -
EOF

check_x "node_child_overflow" "rb" "invalid key or child format/bounds" << 'EOF'
root 5
5 B 9223372036854775808 -
EOF

check_x "node_child_non_numeric" "rb" "invalid key or child format/bounds" << 'EOF'
root 5
5 B x -
EOF

check_x "wrong_attr_rb" "rb" "invalid RB attribute '1'" << 'EOF'
root 5
5 1 - -
EOF

check_x "wrong_attr_avl" "avl" "invalid AVL attribute 'R'" << 'EOF'
root 5
5 R - -
EOF

check_x "double_space" "rb" "expected 4" << 'EOF'
root 5
5  B - -
EOF

check_x "trailing_space" "rb" "expected 4" << 'EOF'
root 5
5 B - - 
EOF

echo "$fails miss(es)"
[ "$fails" -eq 0 ]