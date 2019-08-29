
# Program

A **Program** can only be made of Statements.

### Syntax:

    + ── beginFile ─╮────────────── > ──────────────╭─ endFile ── +
                    |  ╭─────────── < ───────────╮  |
                    ╰──╰────── [Statement] ──────╯──╯

### Tokens used in this Rail (Regexes):

- **beginFile** - Defined as `^`
- **endFile** - Defined as `$`

### Declarations used in this Rail:

- [**Statement**](Statement.md)
