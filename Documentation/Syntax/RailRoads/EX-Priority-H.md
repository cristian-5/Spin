
# High Priority

A **High Priority Expression** is a single instruction that applies
a unary operator or evaluates a **Primary Expression**.

### Syntax:

    + ─╮─ positive ──╭─ [High Priority] ─╭─ +
       ╰─ negative ──╯                   |
       ╰─ negation ──╯                   |
       ╰─ inversion ─╯                   |
       |                                 |
       ╰─────────── [Primary] ───────────╯

### Tokens used in this Rail (Regexes):

- **positive** - Defined as `+`
- **negative** - Defined as `-`
- **negation** - Defined as `!`
- **inversion** - Defined as `~`

### Declarations used in this Rail:

- [**Primary**](EX-Primary.md)
