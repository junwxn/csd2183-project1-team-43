#!/usr/bin/env python3
import os
import re
import subprocess
import sys

BIN = os.environ.get("BIN", "./tree_debug")
KINDS = os.environ.get("KINDS", "rb avl").split()
TIMEOUT = float(os.environ.get("TIMEOUT", "60"))
SAN = ("AddressSanitizer", "LeakSanitizer", "UndefinedBehaviorSanitizer",
       "runtime error")

INT = r"-?\d+"
SP = re.compile(rf"S {INT} (?:found {INT}|absent)|P(?: {INT})*")
X_UNSUPPORTED = re.compile(r"X unsupported")
X_SHAPE = re.compile(rf"root (?:{INT}|-)"
                     rf"|{INT} (?:R|B|-1|0|1) (?:{INT}|-) (?:{INT}|-)")


def run(kind, path):
    result = subprocess.run([BIN, kind, path], capture_output=True,
                            text=True, timeout=TIMEOUT)
    return result.returncode, result.stdout, result.stderr


def split(out, kind):
    x_ok = X_UNSUPPORTED if kind == "map" else X_SHAPE
    sp, unknown = [], []
    for line in out.splitlines():
        if SP.fullmatch(line):
            sp.append(line)
        elif not x_ok.fullmatch(line):
            unknown.append(line)
    return sp, unknown


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
    expected, unknown = split(out, "map")
    if code != 0:
        print(f"ERROR {path}: reference (map) exit code {code}")
        return 2
    if san(err):
        print(f"ERROR {path}: reference (map) sanitizer report: {san(err)}")
        return 2
    if unknown or (out and not out.endswith("\n")):
        print(f"ERROR {path}: reference (map) output is malformed")
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
        actual, unknown = split(out, kind)
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
    return status


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("ERROR: no command files given")
        sys.exit(2)
    status = max(check(path) for path in sys.argv[1:])
    print(("PASS", "FAILED", "ERROR")[status])
    sys.exit(status)