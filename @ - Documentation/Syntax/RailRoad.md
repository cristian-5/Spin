
# Stack Syntax RailRoad

Here's a **railway** implementation of statements' syntax.

- **Keywords** are defined as characters sequences.
- **Refereces** to other railways are enclosed in square brackets.
- **Symbols** are defined as isolated characters.

### If Statement:

    + ── if ── ( ── [Expression] ── ) ── [Statements] ─╮
    ╭──────────────────────────────────────────────────╯
    ╰────────── else ─╮── [If Statement] ──╭────────── +
                      ╰─── [Statements] ───╯

### Switch Statement:

### Repeat While, Do While Statements:

    + ──╮── repeat ──╭── { ── [Statements] ── } ─╮
        ╰──── do ────╯                           |
    ╭────────────────────────────────────────────╯
	╰── while ── ( ── [Expression] ── ) ── ; ─── +

### Repeat Until, Do Until Statements:

    + ──╮── repeat ──╭── { ── [Statements] ── } ─╮
        ╰──── do ────╯                           |
    ╭────────────────────────────────────────────╯
	╰── until ── ( ── [Expression] ── ) ── ; ─── +

### Repeat, Loop Statements:

    + ──╮── repeat ──╭── { ── [Statements] ── } ── +
        ╰─── loop ───╯
