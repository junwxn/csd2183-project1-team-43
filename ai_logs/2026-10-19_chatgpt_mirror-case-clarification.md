# Chat export: CLRS Red-Black Tree Insert Fixup Mirror Cases https://chatgpt.com/share/6ad60495-17e4-83ec-9ff3-1fec075bacca

## User

what are mirror cases in CLRS's insert fixups?

## Assistant

In **CLRS (Introduction to Algorithms)**, *mirror cases* in Red-Black Tree insertion fixup (`RB-INSERT-FIXUP`) are the **left-right symmetric versions of the original three cases**.

They exist because the red-black tree violation can occur on either side of the grandparent.

### 1. Why do mirror cases exist?

When inserting a new red node `z`, we may violate the red-black property:

> A red node cannot have a red parent.

CLRS first describes three cases assuming:

**`z.p` is the LEFT child of `z.p.p` (its grandparent).**

Then it says the `else` branch handles the mirror cases, where:

**`z.p` is the RIGHT child of `z.p.p`.**

The logic is identical, but **left and right are swapped**.

### 2. The three cases and their mirrors

| Case | Original (parent on left) | Mirror (parent on right) |
|---|---|---|
| Case 1 | Uncle is red | Uncle is red |
| Case 2 | `z` is right child (triangle) | `z` is left child (triangle) |
| Case 3 | `z` is left child (line) | `z` is right child (line) |

#### Case 1: Uncle is red

Original:

```text
       G(B)
      /    \
    P(R)   U(R)
   /
 z(R)
```

Mirror:

```text
       G(B)
      /    \
    U(R)   P(R)
              \
              z(R)
```

**Fix:** Recolor parent and uncle black, grandparent red, and move `z` up to the grandparent. No rotation.

#### Case 2: Triangle configuration

Original (left-right):

```text
       G(B)
      /
    P(R)
       \
       z(R)
```

Left-rotate at P.

Mirror (right-left):

```text
       G(B)
          \
          P(R)
          /
        z(R)
```

Right-rotate at P.

**Fix:** Rotate at the parent to transform the triangle into Case 3.

#### Case 3: Line configuration

Original (left-left):

```text
        G(B)
        /
      P(R)
      /
    z(R)
```

Right-rotate at G.

Mirror (right-right):

```text
    G(B)
       \
       P(R)
          \
          z(R)
```

Left-rotate at G.

**Fix:** Recolor parent black, grandparent red, then rotate at the grandparent.

### 3. How this appears in CLRS pseudocode

The original branch is:

```python
if z.p == z.p.p.left:
    y = z.p.p.right  # uncle

    # Cases 1, 2, 3
```

The mirror branch is:

```python
else:
    y = z.p.p.left  # uncle

    # Mirror Cases 1, 2, 3
```

Everything is symmetrical:

- `left` becomes `right`
- `right` becomes `left`
- `LEFT-ROTATE` becomes `RIGHT-ROTATE`
- `RIGHT-ROTATE` becomes `LEFT-ROTATE`

**Key takeaway:** Mirror cases are not three additional algorithms to memorize. They are the same three cases, reflected horizontally.

An easy rule to remember: **Red uncle → recolor. Black uncle + triangle → rotate parent. Black uncle + line → recolor and rotate grandparent.**

## User

export this chat as markdown (including this)

---

*Export note: The original answer displayed tree diagrams as graphics. This Markdown export represents them as text diagrams, preserving their meaning.*
