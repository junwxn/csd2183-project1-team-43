# Sourced by test_parser.sh and test_traversal.sh; they set fails=0 first.
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