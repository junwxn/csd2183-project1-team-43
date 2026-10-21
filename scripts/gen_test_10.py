#!/usr/bin/env python3
import os
import random

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "tests", "10_random_churn_memory_stress.txt")


def generate_test_10(filename=OUT, seed=2183):
    os.makedirs(os.path.dirname(filename), exist_ok=True)

    KEYS = 500
    SORTED_DELETES = 250
    RANDOM_COMMANDS = 8500

    rng = random.Random(seed)
    lines = [
        "# Target: sorted insertion and one-end deletion, then random churn over "
        "keys 1 to 500 (dense collisions, absent deletes), then drain every key "
        "to check for memory leaks."
    ]
    live_keys = set()

    for key in range(1, KEYS + 1):
        lines.append(f"I {key} {key}")
        live_keys.add(key)
    lines += ["P", "X"]
    for key in range(1, SORTED_DELETES + 1):
        lines.append(f"D {key}")
        live_keys.discard(key)
    lines += ["P", "X"]

    for i in range(1, RANDOM_COMMANDS + 1):
        op = rng.choices("IDS", weights=(50, 30, 20))[0]
        key = rng.randint(1, KEYS)
        if op == "I":
            lines.append(f"I {key} {rng.randint(-10**6, 10**6)}")
            live_keys.add(key)
        elif op == "D":
            lines.append(f"D {key}")
            live_keys.discard(key)
        else:
            lines.append(f"S {key}")
        if i % 2000 == 0:
            lines += ["P", "X"]

    print(f"Sanity Check: {len(live_keys)} keys alive at the end of random phase.")

    drain = list(range(1, KEYS + 1))
    rng.shuffle(drain)
    lines += [f"D {key}" for key in drain]
    lines.append("X")

    commands = [line for line in lines if not line.startswith("#")]
    assert len(commands) <= 10000, f"Command count {len(commands)} exceeds 10,000 limit"

    with open(filename, "w", newline="\n") as file:
        file.write("\n".join(lines) + "\n")

    print(f"Wrote {filename}: {len(commands)} commands")


if __name__ == "__main__":
    generate_test_10()