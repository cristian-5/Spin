
# Function Declaration

A **Function** declares a function in scope.

### Syntax:

    + ── func ── identifier ── ( ─╮────────────────────╭─ ) ─╮
                                  ╰─── [Parameters] ───╯     |
    ╭────────────────────────────────────────────────────────╯
    ╰─────────── arrow ───── [Type] ─────  [Block] ───────── +

### Tokens used in this Rail (Regexes):

- **func** - Defined as `func`
- **identifier** - Defined as `[A-Za-z_][A-Za-z0-9_]*`
- **arrow** - Defined as `->`

### Declarations used in this Rail:

- [**Block**](ST-Block.md)
- [**Parameters**](DC-Parameters.md)
- [**Type**](DC-Type.md)
