
# Comparison

A **Comparison Expression** is a single instruction
that compares two **Low Priority Expressions**.

### Syntax:

    + ── [Low Priority] ─╮─── major ────╭─ [Low Priority] ── +
                         ╰─── minor ────╯
                         ╰─ majorEqual ─╯
                         ╰─ minorEqual ─╯

### Tokens used in this Rail (Regexes):

- **major** - Defined as `>`
- **minor** - Defined as `<`
- **majorEqual** - Defined as `>=`
- **minorEqual** - Defined as `<=`

### Declarations used in this Rail:

- [**Low Priority**](EX-Priority-L.md)
