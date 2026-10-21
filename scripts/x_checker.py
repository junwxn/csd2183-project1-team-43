#!/usr/bin/env python3
import sys
import re

class TreeError(Exception): pass

# fullmatch() requires no anchors. [0-9] prevents Unicode digit bypasses.
INT_RE = re.compile(r"0|-?[1-9][0-9]*")
MIN_INT64, MAX_INT64 = -2**63, 2**63 - 1

def parse_tree(lines, kind):
    if kind not in {"rb", "avl"}:
        raise ValueError(f"invalid tree kind: {kind}")

    nodes = {}
    root_key, last_key = None, None

    for line in lines:
        # Safely remove Windows \r and standard \n without hiding empty strings
        line = line.rstrip("\r\n")
        
        if not line:
            raise TreeError("blank line encountered")
        if line.startswith("#"):
            raise TreeError("comments not allowed in X output block")
            
        parts = line.split(" ") 

        if parts[0] == "root":
            if root_key is not None:
                raise TreeError("multiple root lines")
            if len(parts) != 2:
                raise TreeError(f"root line has {len(parts)} fields, expected 2 (check for trailing/double spaces)")
            
            key_str = parts[1]
            if key_str == "-":
                root_key = "-"
            elif INT_RE.fullmatch(key_str) and MIN_INT64 <= int(key_str) <= MAX_INT64:
                root_key = int(key_str)
            else:
                raise TreeError(f"invalid root key format or bounds: {key_str}")

        else:
            if root_key is None:
                raise TreeError("node line appears before root line")
            if len(parts) != 4:
                raise TreeError(f"node line has {len(parts)} fields, expected 4 (check for trailing/double spaces)")

            key_str, attr, left_str, right_str = parts
            
            if key_str == "-":
                raise TreeError("node key cannot be '-'")

            for s in (key_str, left_str, right_str):
                if s != "-" and not (INT_RE.fullmatch(s) and MIN_INT64 <= int(s) <= MAX_INT64):
                    raise TreeError(f"invalid key or child format/bounds: {s}")

            key = int(key_str)
            left = "-" if left_str == "-" else int(left_str)
            right = "-" if right_str == "-" else int(right_str)

            if last_key is not None:
                if key == last_key:
                    raise TreeError(f"duplicate key {key}")
                if key < last_key:
                    raise TreeError(f"node lines not ascending: {last_key} then {key}")

            if kind == "avl" and attr not in {"-1", "0", "1"}:
                raise TreeError(f"invalid AVL attribute {attr!r}")
            if kind == "rb" and attr not in {"R", "B"}:
                raise TreeError(f"invalid RB attribute {attr!r}")

            if left == right and left != "-":
                raise TreeError(f"node {key} has same child {left} on both sides")
            if key == left or key == right:
                raise TreeError(f"node {key} lists itself as a child")

            nodes[key] = (attr, left, right)
            last_key = key

    if root_key is None:
        raise TreeError("missing root line")
    if root_key == "-" and nodes:
        raise TreeError("root is - but node lines exist")
    if root_key != "-" and root_key not in nodes:
        raise TreeError(f"root key {root_key} not in nodes")
        
    return root_key, nodes

def validate(kind, root, nodes):
    if root == "-": return
    if kind == "rb" and nodes[root][0] != "B":
        raise TreeError(f"root {root} is not black")

    visited = set()
    
    def dfs(key, low, high):
        if key == "-": return 0, 1  # (height, black-height)
        
        if key in visited:
            raise TreeError(f"cycle or shared child at {key}")
        if key not in nodes:
            raise TreeError(f"dangling child {key}")
        
        visited.add(key)
        int_k = int(key)
        
        if not (low < int_k < high):
            raise TreeError(f"BST violation at {key} (not in range {low} to {high})")
        
        attr, left, right = nodes[key]
        
        hl, bhl = dfs(left, low, int_k)
        hr, bhr = dfs(right, int_k, high)

        if kind == "avl":
            bf = hr - hl
            if int(attr) != bf:
                unbalanced = " (unbalanced)" if abs(bf) > 1 else ""
                raise TreeError(f"node {key}: stated factor {attr}, true factor {bf}{unbalanced}")
            return max(hl, hr) + 1, 0
        else:
            if bhl != bhr:
                raise TreeError(f"node {key}: left black-height {bhl}, right black-height {bhr}")
            if attr == "R":
                for child in (left, right):
                    if child != "-" and nodes[child][0] == "R":
                        raise TreeError(f"red node {key} has red child {child}")
                return 0, bhl
            return 0, bhl + 1

    try:
        dfs(root, MIN_INT64 - 1, MAX_INT64 + 1)
    except RecursionError:
        raise TreeError("recursion limit exceeded (degenerate tree)")

    if len(visited) != len(nodes):
        raise TreeError("unreachable nodes detected")

if __name__ == "__main__":
    if len(sys.argv) < 2: sys.exit("usage: x_checker.py <rb|avl>")
    try:
        root_key, parsed_nodes = parse_tree(sys.stdin, sys.argv[1])
        validate(sys.argv[1], root_key, parsed_nodes)
        print("PASS")
    except TreeError as err:
        print(f"FAIL: {err}", file=sys.stderr)
        sys.exit(1)