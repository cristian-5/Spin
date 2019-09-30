
# Procedure Declaration

A **Procedure** declares a procedure in scope.

### Syntax:

    + ── proc ── identifier ── ( ─╮────────────────────╭─ ) ──  [Block] ── +
                                  ╰─── [Parameters] ───╯

### Tokens used in this Rail (Regexes):

- **proc** - Defined as `proc`
- **identifier** - Defined as `[A-Za-z_][A-Za-z0-9_\-]*`

### Declarations used in this Rail:

- [**Block**](ST-Block.md)
- [**Parameters**](DC-Parameters.md)
