
# Medium Priority

A **Medium Priority Expression** is a single instruction
that multiplies, divides, applies modulus between
two **High Priority Expressions**.

### Syntax:

    + ── [High Priority] ─╮─ multiplication ─╭─ [High Priority] ── +
                          ╰─ division ───────╯
                          ╰─ modulus ────────╯

### Tokens used in this Rail (Regexes):

- **multiplication** - Defined as `*`
- **division** - Defined as `/`
- **modulus** - Defined as `%`

### Declarations used in this Rail:

- [**High Priority**](EX-Priority-H.md)
