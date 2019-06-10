
# File

A **File** can only be made of Import Statements,
Libraries, Declarations and one Entry Point.

### Syntax:

    + ── beginFile ─╮────────────── > ──────────────╭─ endFile ── +
                    |  ╭─────────── < ───────────╮  |
                    ╰──╰─╮─ [Import Statement] ──╯──╯
                    |    ╰─ [Library] ──────────────╯
                    |    ╰─ [Declaration] ──────────╯
                    ╰────── [Entry Point] ──────────╯

### Tokens used in this Rail (Regexes):

- **beginFile** - Defined as `^`
- **endFile** - Defined as `$`

### Declarations used in this Rail:

- [**Import Statement**](Import-Statement.md)
- [**Library**](Library.md)
- [**Declaration**](Declaration.md)
- [**Entry Point**](Entry-Point.md)
