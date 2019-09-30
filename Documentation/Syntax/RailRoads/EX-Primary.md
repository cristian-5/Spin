
# Primary

A **Primary Expression** is a single instruction
that represents a **Literal** or evaluates a
**Grouping Expression**.

### Syntax:

    + ─╮── [Array] ────╭─ +
       ╰── [Grouping] ─╯
       ╰── [Literal] ──╯
       ╰── identifier ─╯

### Tokens used in this Rail (Regexes):

- **identifier** - Defined as `[A-Za-z_][A-Za-z0-9_\-]*`

### Declarations used in this Rail:

- [**Array**](EX-Array.md)
- [**Grouping**](EX-Grouping.md)
- [**Literal**](EX-Literal.md)
