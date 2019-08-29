
# Low Priority

A **Low Priority Expression** is a single instruction
that adds or subtracts two **Medium Priority Expressions**.

### Syntax:

    + ── [Medium Priority] ─╮─ addition ────╭─ [Medium Priority] ── +
                            ╰─ subtraction ─╯

### Tokens used in this Rail (Regexes):

- **addition** - Defined as `+`
- **subtraction** - Defined as `-`

### Declarations used in this Rail:

- [**Medium Priority**](EX-Priority-M.md)
