
# Switch Statement

Basic example of a `switch` statement.
Note that the switch statement will result
in an optimised version of consecutive
[if statements](If-Statement.md) comparing
the top expression with every other case,
using the **comparison operator** `==`.
The main expression will be evaluated just
once but the other are compared as they go.
The [break statement](Break-Statement.md) is
not required as long as you don't want to
skip instructions and exit the switch.
Since statement contains [code block](Code-Block.md)
you can create a context when you need declarations.

### Syntax:

    + ──── switch ─── ( ── [Expression] ── ) ─── > ───╮
    ╭─── < ────────────────── < ──────────────── < ───╯
    ╰── { ─╭─ case ── [Expression] ─ : ─ [Statement] ─╮──╮
           ╰─────────────────── < ────────────────────╯  |
    ╭────────── < ───────────── < ───────────── < ───────╯
    ╰─── > ────╮───────────── > ─────────────╭── } ── +
               ╰─ default ─ : ─ [Statement] ─╯

### Tokens used in this Rail (Regexes):

- **switch** - Defined as `switch`
- **case** - Defined as `case`
- **default** - Defined as `default`

### Declarations used in this Rail:

- [**Expression**](Expression.md)
- [**Statement**](Statement.md)
