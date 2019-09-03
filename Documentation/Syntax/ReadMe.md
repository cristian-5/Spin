
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
- [**Expression Statement**](RailRoads/ST-Expression.md)
- [**If Statement**](RailRoads/ST-If.md)
- [**Print Statement**](RailRoads/ST-Print.md)
- [**While Statement**](RailRoads/ST-While.md)





### Expression Statement:

    + ── [Expression] ── ; ── +

### Statements:

    + ─╭─ [Statement] ─╮─ +
       ╰────── < ──────╯

### Return Statement:

    + ── return ──╮────── > ───────╭── ; ── +
                  ╰─ [Expression] ─╯

### Break Statement:

    + ── break ── ; ── +

### Continue Statement:

    + ── continue ── ; ── +

### Subscription:

    + ── [Expression] ── [ ── [Expression] ── ] ── +

### Expression Precedence:

    + ── ( ── [Expression] ── ) ── +

### Expression:

    + ─╮── > ── [Literals] ───────────────── > ──╭─ +
	   |                                         |
       ╰── > ── [Expression Precedence] ──── > ──╯
	   |                                         |
	   ╰── > ── [Subscription] ───────────── > ──╯
	   |                                         |
       ╰── > ── [Assignment] ─────────────── > ──╯
       |                                         |
	   ╰── > ── [Prefix Operation] ───────── > ──╯
       ╰── > ── [Infix Operation] ────────── > ──╯
	   ╰── > ── [Postfix Operation] ──────── > ──╯
	   |                                         |
	   ╰── > ── [Function Call] ──────────── > ──╯

### Assignment:

    + ── [Expression] ── = ── [Expression] ── +

### Prefix Operation:

    + ── prefixOperator ── [Expression] ── +

### Infix Operation:

    + ── [Expression] ── infixOperator ── [Expression] ── +

### Literal:

    + ─╮── > ── stringLiteral ───── > ──╭─ +
       ╰── > ── integerLiteral ──── > ──╯
       ╰── > ── charLiteral ─────── > ──╯
       ╰── > ── boolLiteral ─────── > ──╯
       ╰── > ── realLiteral ─────── > ──╯

### Switch Statement:

    + ──── switch ─── ( ── [Expression] ── ) ─── > ───╮
    ╭─── < ────────────────── < ──────────────── < ───╯
    ╰── { ─╭─ case ── [Expression] ─ : ─ [Statement] ─╮──╮
           ╰─────────────────── < ────────────────────╯  |
    ╭────────── < ───────────── < ───────────── < ───────╯
    ╰─── > ────╮───────────── > ─────────────╭── } ── +
               ╰─ default ─ : ─ [Statement] ─╯

### Do While Statement:

    + ─╮─ repeat ──╭── > ── [Statement] ── > ───╮
       ╰─── do ────╯                            |
    ╭────── < ─────────── < ─────────── < ──────╯
    ╰── while ── ( ── [Expression] ── ) ── ; ── +

### Repeat Until Statement:

    + ─╮─ repeat ──╭── > ── [Statement] ── > ───╮
       ╰─── do ────╯                            |
    ╭────── < ─────────── < ─────────── < ──────╯
    ╰── until ── ( ── [Expression] ── ) ── ; ── +

### Loop Statement:

    + ─╮─ repeat ──╭── > ── [Statement] ── > ── +
       ╰── loop ───╯

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

## Declarations

This part focuses on *"objects"* declaration.







