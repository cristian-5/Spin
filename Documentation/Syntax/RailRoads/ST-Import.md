
# Import Statement

A **Import Statement** is made of an `import`
token and a library name followed by semicolon.

### Syntax:

    + ── import ─╭── identifier ─╮─ ; ── +
                 ╰─ < ── . ── < ─╯

### Tokens used in this Rail (Regexes):

- **import** - Defined as `import`
- **identifier** - Defined as `[A-Za-z_][A-Za-z0-9_\-]*`
