
# Stack Syntax RailRoad

Here's a **railway** implementation of statements' syntax.

- **Keywords** are defined as characters sequences.
- **Refereces** to other railways are enclosed in square brackets.
- **Symbols** are defined as isolated characters.
- **Loops** (self references) are defined as loops and arrows.

### Expression:
### Statement:

### Statements:

    + ─╭─ [Statement] ─╮─ +
       ╰────── < ──────╯

### If Statement:

    + ── if ── ( ── [Expression] ── ) ─╮
    ╭──────────────────────────────────╯
    ╰──╮─────── [Statement] ────────╭─╮
       ╰─ { ─╭─ [Statement] ─╮─ } ──╯ |
             ╰────── < ──────╯        |
    ╭─────────────────────────────────╯
    ╰─ else ─╮── [If Statement] ──╭─────────────╭── +
             ╰──╮──────── [Statement] ────────╭─╯
                ╰── { ─╭─ [Statement] ─╮─ } ──╯
                       ╰────── < ──────╯

### Switch Statement:
### Repeat While, Do While Statements:
### Repeat Until, Do Until Statements:
### Repeat, Loop Statements:
### Try Catch Finally Statements:
