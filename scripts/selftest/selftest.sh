#!/bin/bash
cd "$(dirname "$0")/.." || exit 2
FILE=tests/example.txt
fails=0
UBSAN="src/red_black_tree.cpp:123:45: runtime error: signed integer overflow"
ASAN="==4242==ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010"

expect() {
    if [ "$code" -eq "$2" ] && grep -qxF -- "$3" <<< "$out"; then
        echo "ok    $1"
    else
        echo "MISS  $1 (exit=$code, wanted exit $2 and line: $3)"
        echo "$out"
        fails=$((fails + 1))
    fi
}

check() {
    out=$(TAMPER=$1 BIN=scripts/selftest/tamper.py KINDS=rb TIMEOUT=2 \
        python3 scripts/validate.py "$FILE")
    code=$?
    if grep -qxF "FAIL $FILE [rb]: exit code 99" <<< "$out"; then
        echo "SETUP ${1:-no tamper} (fault was not applied)"
        fails=$((fails + 1))
    else
        expect "${1:-no tamper}" "$2" "$3"
    fi
}

out=$(KINDS=map python3 scripts/validate.py "$FILE"); code=$?
expect "map vs map" 0 "PASS"

check                       "" 0 "PASS"
check benign_stderr 0       "PASS"
check wrong_found_first 1   "FAIL $FILE:7 [rb]: map='S 5 found 50' got='S 5 absent'"
check wrong_value 1         "FAIL $FILE:7 [rb]: map='S 5 found 50' got='S 5 found 51'"
check drop_first_s 1        "FAIL $FILE:7 [rb]: map='S 5 found 50' got='S 5 absent'"
check drop_last_s 1         "FAIL $FILE:11 [rb]: map='S 5 found 52' got='P 5'"
check extra_s 1             "FAIL $FILE [rb]: line count map=4 got=5"
check change_p_key 1        "FAIL $FILE:12 [rb]: map='P 5' got='P 6'"
check stray 1               "FAIL $FILE [rb]: unexpected output line 'debug: hello'"
check x_unsupported 1       "FAIL $FILE [rb]: unexpected output line 'X unsupported'"
check no_newline 1          "FAIL $FILE [rb]: missing final newline"
check exit1 1               "FAIL $FILE [rb]: exit code 1"
check hang 1                "FAIL $FILE [rb]: timeout"
check ubsan 1               "FAIL $FILE [rb]: sanitizer report: $UBSAN"
check asan 1                "FAIL $FILE [rb]: sanitizer report: $ASAN"

out=$(BIN=/bin/false python3 scripts/validate.py "$FILE"); code=$?
expect "broken reference" 2 "ERROR $FILE: reference (map) exit code 1"

out=$(BIN=/no/such/program python3 scripts/validate.py "$FILE"); code=$?
expect "missing program" 2 "ERROR $FILE: cannot run /no/such/program: No such file or directory"

echo "$fails miss(es)"
[ "$fails" -eq 0 ]