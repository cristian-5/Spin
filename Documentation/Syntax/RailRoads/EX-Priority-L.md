
# Low Priority

A **Low Priority Expression** is a single instruction that
applies an operator between two **Medium Priority Expressions**.

### Syntax:

    + ── [Medium Priority] ─╮─ addition ────╭─ [Medium Priority] ── +
                            ╰─ subtraction ─╯
                            ╰─ or ──────────╯

### Tokens used in this Rail (Regexes):

- **addition** - Defined as `+`
- **subtraction** - Defined as `-`
- **or** - Defined as `|`

### Declarations used in this Rail:

- [**Medium Priority**](EX-Priority-M.md)
