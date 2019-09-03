
# Program

A **Program** can only be made of Declarations.

### Syntax:

    + ── beginFile ─╮────────────── > ──────────────╭─ endFile ── +
                    |  ╭─────────── < ───────────╮  |
                    ╰──╰───── [Declaration] ─────╯──╯

### Tokens used in this Rail (Regexes):

- **beginFile** - Defined as `^`
- **endFile** - Defined as `$`

### Declarations used in this Rail:

- [**Declaration**](Declaration.md)
