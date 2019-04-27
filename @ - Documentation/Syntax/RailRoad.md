
# Stack Syntax RailRoad

Here's a *railway* implementation of statements' syntax.

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
