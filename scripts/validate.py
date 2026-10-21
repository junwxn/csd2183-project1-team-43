#!/usr/bin/env python3
import os
import re
import subprocess
import sys
import x_checker

BIN = os.environ.get("BIN", "./tree_debug")
KINDS = os.environ.get("KINDS", "rb avl").split()
TIMEOUT = float(os.environ.get("TIMEOUT", "60"))
SAN = ("AddressSanitizer", "LeakSanitizer", "UndefinedBehaviorSanitizer",
       "runtime error")

VALID_KINDS = {"rb", "avl", "map"}
for k in KINDS:
    if k not in VALID_KINDS:
        print(f"ERROR: unknown tree kind {k!r}")
        sys.exit(2)

INT = r"-?\d+"
SP = re.compile(rf"S {INT} (?:found {INT}|absent)|P(?: {INT})*")
X_UNSUPPORTED = re.compile(r"X unsupported")
X_SHAPE = re.compile(rf"root (?:{INT}|-)"
                     rf"|{INT} (?:R|B|-1|0|1) (?:{INT}|-) (?:{INT}|-)")

def get_expected_x_states(path):
    expected_keys = set()
    states = []
    with open(path) as file:
        for lineno, line in enumerate(file, 1):
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = line.split()
            cmd = parts[0]
            if cmd == 'I':
                expected_keys.add(int(parts[1]))
            elif cmd == 'D':
                expected_keys.discard(int(parts[1]))
            elif cmd == 'X':
                states.append((lineno, sorted(expected_keys)))
            elif cmd not in ('S', 'P'):
                print(f"ERROR {path}:{lineno}: unknown command {cmd}")
                sys.exit(2)
    return states

def run(kind, path):
    result = subprocess.run([BIN, kind, path], capture_output=True,
                            text=True, timeout=TIMEOUT)
    return result.returncode, result.stdout, result.stderr


def split(out, kind):
    x_ok = X_UNSUPPORTED if kind == "map" else X_SHAPE
    sp, unknown, x_blocks = [], [], []
    current_x = []
    
    for line in out.splitlines():
        if SP.fullmatch(line):
            if current_x:
                x_blocks.append(current_x)
                current_x = []
            sp.append(line)
        elif not x_ok.fullmatch(line):
            unknown.append(line)
        else:
            if kind == "map":
                x_blocks.append([line])
            else:
                if line.startswith("root "):
                    if current_x:
                        x_blocks.append(current_x)
                    current_x = [line]
                else:
                    current_x.append(line)
                    
    if current_x:
        x_blocks.append(current_x)
        
    return sp, unknown, x_blocks


def command_lines(path):
    with open(path) as file:
        return [n for n, line in enumerate(file, 1) if line[:1] in ("S", "P")]


def san(err):
    for line in err.splitlines():
        if any(marker in line for marker in SAN):
            return line.strip()
    return ""


def check(path):
    try:
        code, out, err = run("map", path)
    except subprocess.TimeoutExpired:
        print(f"ERROR {path}: reference (map) timed out")
        return 2
    except OSError as problem:
        print(f"ERROR {path}: cannot run {BIN}: {problem.strerror}")
        return 2
    
    expected, unknown, map_x_blocks = split(out, "map")
    
    if code != 0:
        print(f"ERROR {path}: reference (map) exit code {code}")
        return 2
    if san(err):
        print(f"ERROR {path}: reference (map) sanitizer report: {san(err)}")
        return 2
    if unknown or (out and not out.endswith("\n")):
        print(f"ERROR {path}: reference (map) output is malformed")
        return 2

    expected_x_states = get_expected_x_states(path)
    
    if len(map_x_blocks) != len(expected_x_states):
        print(f"ERROR {path}: reference (map) X block count: expected {len(expected_x_states)} got {len(map_x_blocks)}")
        return 2

    status = 0
    numbers = command_lines(path)
    for kind in KINDS:
        try:
            code, out, err = run(kind, path)
        except subprocess.TimeoutExpired:
            print(f"FAIL {path} [{kind}]: timeout")
            status = 1
            continue
        except OSError as problem:
            print(f"ERROR {path}: cannot run {BIN}: {problem.strerror}")
            return 2
        if code != 0:
            print(f"FAIL {path} [{kind}]: exit code {code}")
            status = 1
        report = san(err)
        if report:
            print(f"FAIL {path} [{kind}]: sanitizer report: {report}")
            status = 1
        if out and not out.endswith("\n"):
            print(f"FAIL {path} [{kind}]: missing final newline")
            status = 1
        actual, unknown, x_blocks = split(out, kind)
        if unknown:
            print(f"FAIL {path} [{kind}]: unexpected output line "
                  f"{unknown[0]!r}")
            status = 1
        if actual != expected:
            status = 1
            for i, (want, got) in enumerate(zip(expected, actual)):
                if want != got:
                    print(f"FAIL {path}:{numbers[i]} [{kind}]: "
                          f"map={want!r} got={got!r}")
                    break
            else:
                print(f"FAIL {path} [{kind}]: line count "
                      f"map={len(expected)} got={len(actual)}")

        if kind != "map":
            if len(x_blocks) != len(expected_x_states):
                print(f"FAIL {path} [{kind}]: X block count: expected {len(expected_x_states)} got {len(x_blocks)}")
                status = 1
            else:
                for block, (x_lineno, expected_keys) in zip(x_blocks, expected_x_states):
                    try:
                        root_key, parsed_nodes = x_checker.parse_tree(block, kind)
                        actual_keys = sorted(parsed_nodes.keys())
                        
                        if expected_keys != actual_keys:
                            missing = sorted(set(expected_keys) - set(actual_keys))
                            extra = sorted(set(actual_keys) - set(expected_keys))
                            raise x_checker.TreeError(f"key set mismatch. missing={missing} extra={extra}")
                            
                        x_checker.validate(kind, root_key, parsed_nodes)
                            
                    except (x_checker.TreeError, RecursionError) as e:
                        msg = "maximum recursion depth exceeded (possible cycle)" if isinstance(e, RecursionError) else str(e)
                        print(f"FAIL {path}:{x_lineno} [{kind}]: {msg}")
                        status = 1
                        break   
    return status


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("ERROR: no command files given")
        sys.exit(2)
    status = max(check(path) for path in sys.argv[1:])
    print(("PASS", "FAILED", "ERROR")[status])
    sys.exit(status)
