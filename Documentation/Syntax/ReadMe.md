
# Stack Syntax RailRoad

Here's a **railway** implementation of statements' syntax.

- **Tokens** are defined as regex objects.
- **References** to other railways are enclosed in square brackets.
- **Symbols** are defined as isolated characters.
- **Loops** (self references) are defined as loops and arrows.

### Index

- [**Expression**](RailRoads/Expression.md)

- [**Equality**](RailRoads/EX-Equality.md)
- [**Comparison**](RailRoads/EX-Comparison.md)

- [**Low Priority**](RailRoads/EX-Priority-L.md)
- [**Medium Priority**](RailRoads/EX-Priority-M.md)
- [**High Priority**](RailRoads/EX-Priority-H.md)

- [**Primary**](RailRoads/EX-Primary.md)

- [**Grouping**](RailRoads/EX-Grouping.md)
- [**Literal**](RailRoads/EX-Literal.md)


- [**Program**](RailRoads/Program.md)

- [**Declaration**](RailRoads/Declaration.md)
- [**Type**](RailRoads/DC-Type.md)
- [**Variable Declaration**](RailRoads/DC-Variable.md)


- [**Statement**](RailRoads/Statement.md)

- [**Block Statement**](RailRoads/ST-Block.md)
- [**Break Statement**](RailRoads/ST-Break.md)
- [**Continue Statement**](RailRoads/ST-Continue.md)
- [**Do While Statement**](RailRoads/ST-Do-While.md)
- [**Expression Statement**](RailRoads/ST-Expression.md)
- [**If Statement**](RailRoads/ST-If.md)
- [**Print Statement**](RailRoads/ST-Print.md)
- [**Repeat Until Statement**](RailRoads/ST-Repeat-Until.md)
- [**Until Statement**](RailRoads/ST-Until.md)
- [**While Statement**](RailRoads/ST-While.md)




### Return Statement:

    + ── return ──╮────── > ───────╭── ; ── +
                  ╰─ [Expression] ─╯

### Subscription:

    + ── [Expression] ── [ ── [Expression] ── ] ── +

### Switch Statement:

    + ──── switch ─── ( ── [Expression] ── ) ─── > ───╮
    ╭─── < ────────────────── < ──────────────── < ───╯
    ╰── { ─╭─ case ── [Expression] ─ : ─ [Statement] ─╮──╮
           ╰─────────────────── < ────────────────────╯  |
    ╭────────── < ───────────── < ───────────── < ───────╯
    ╰─── > ────╮───────────── > ─────────────╭── } ── +
               ╰─ default ─ : ─ [Statement] ─╯

### Try Catch Statement:

    + ────── try ── > ── [Statement] ── > ──╮
    ╭────────────── < ───────────────── < ──╯
    ╰── catch ── ( ── [Expression] ── ) ────╮
    ╭────────────── < ───────────────── < ──╯
    ╰── [Statement] ──╮─── > ───╭────── > ─ +
    ╭─────────────────╯         |
    ╰── finally ── [Statement] ─╯

### Avoid Statement:

    + ── avoid ──── ( ── [Expression] ── ) ───╮
    ╭─── < ───────────── < ───────────── < ───╯
    ╰── [Statement] ──── > ───── > ───── > ── +

### Throw Statement:

    + ── throw ──╮────── > ───────╭─ ; ── +
                 ╰─ [Expression] ─╯
