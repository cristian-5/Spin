
# Medium Priority

A **Medium Priority Expression** is a single instruction that
applies an operator between two **High Priority Expressions**.

### Syntax:

    + ── [High Priority] ─╮─ multiplication ─╭─ [High Priority] ── +
                          ╰─ division ───────╯
                          ╰─ and ────────────╯
                          ╰─ xor ────────────╯
                          ╰─ modulus ────────╯

### Tokens used in this Rail (Regexes):

- **multiplication** - Defined as `*`
- **division** - Defined as `/`
- **and** - Defined as `&`
- **xor** - Defined as `^`
- **modulus** - Defined as `%`

### Declarations used in this Rail:

- [**High Priority**](EX-Priority-H.md)
