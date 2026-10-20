#!/usr/bin/env python3
import os
import subprocess
import sys
import time

REAL = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                    "..", "..", "tree_debug")

#   UndefinedBehaviorSanitizer (UBSan) catches undefined behaviour such as signed integer overflow
UBSAN = "src/red_black_tree.cpp:123:45: runtime error: signed integer overflow\n"

#   AddressSanitizer (ASan) catches memory errors such as use-after-free
ASAN = ("==4242==ERROR: AddressSanitizer: heap-use-after-free "
        "on address 0x602000000010\n")

#   Ordinary diagnostics on stderr, which the spec allows and must not be flagged
BENIGN = "diagnostic: rebalancing finished\n"

#   Prints a harness problem and exits with 99, which the validator never expects
def fail(message):
    sys.stderr.write(f"tamper.py: {message}\n")
    sys.exit(99)



def tamper(mode, lines, err, code):
    s_at = [i for i, line in enumerate(lines) if line.startswith("S ")]
    found_at = [i for i in s_at if " found " in lines[i]]
    p_at = [i for i, line in enumerate(lines) if line.startswith("P ")]

    #   S 5 found 50 becomes S 5 absent
    if mode == "wrong_found_first" and found_at:
        lines[found_at[0]] = "S " + lines[found_at[0]].split()[1] + " absent"

    #   S 5 found 50 becomes S 5 found 51
    elif mode == "wrong_value" and found_at:
        words = lines[found_at[0]].split()
        words[3] = str(int(words[3]) + 1)
        lines[found_at[0]] = " ".join(words)

    #   Drops the first S line
    elif mode == "drop_first_s" and s_at:
        del lines[s_at[0]]

    #   Drops the last S line
    elif mode == "drop_last_s" and s_at:
        del lines[s_at[-1]]

    #   Adds S 999 absent to the end - mismatch output: one line too long
    elif mode == "extra_s":
        lines.append("S 999 absent")

    #   P 5 becomes P 6
    elif mode == "change_p_key" and p_at:
        words = lines[p_at[0]].split()
        words[1] = str(int(words[1]) + 1)
        lines[p_at[0]] = " ".join(words)

    #   Adds a line that isn't any identifiable format
    elif mode == "stray":
        lines.append("debug: hello")

    #   Adds X unsupported, which only map is allowed to print
    elif mode == "x_unsupported":
        lines.append("X unsupported")

    #   Output is unchanged here, main() removes the final newline afterwards
    elif mode == "no_newline" and lines:
        pass

    #   Changes only the exit code
    elif mode == "exit1":
        code = 1

    #   Pauses the program for 30s, longer than validator's timeout of 2 seconds
    elif mode == "hang":
        time.sleep(30)

    #   Append fake UBSan report to stderr
    elif mode == "ubsan":
        err += UBSAN

    #   Append fake ASan report to stderr
    elif mode == "asan":
        err += ASAN

    #   Append harmless diagnostics to stderr - validator must still pass
    elif mode == "benign_stderr":
        err += BENIGN

    else:
        return None
    return lines, err, code


def main():
    if len(sys.argv) < 3:
        fail("usage: tamper.py KIND COMMAND_FILE")

    if not os.path.exists(REAL):
        fail("tree_debug not found, run make debug first")

    kind = sys.argv[1]
    mode = os.environ.get("TAMPER", "")

    result = subprocess.run([REAL, "map"] + sys.argv[2:],
                            capture_output=True, text=True)
    lines = result.stdout.splitlines()
    err, code = result.stderr, result.returncode

    tampered = bool(mode) and kind != "map"
    if tampered:
        damaged = tamper(mode, lines, err, code)

        #   Fault could not be applied - exit 99 so it is never mistaken for a catch
        if damaged is None:
            fail(f"cannot apply mode {mode!r}")
        lines, err, code = damaged

    text = "".join(line + "\n" for line in lines)
    if tampered and mode == "no_newline":
        text = text.rstrip("\n")
    sys.stdout.write(text)
    sys.stderr.write(err)
    sys.exit(code)


main()