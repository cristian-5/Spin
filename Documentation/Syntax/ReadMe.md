
# Stack Syntax RailRoad

Here's a **railway** implementation of statements' syntax.

- **Tokens** are defined as regex objects.
- **References** to other railways are enclosed in square brackets.
- **Symbols** are defined as isolated characters.
- **Loops** (self references) are defined as loops and arrows.

### Index

- [**Statement**](RailRoads/Statement.md)

- [**No Operation**](RailRoads/No-Operation.md)

- [**If Statement**](If-Statement.md)
- [**Switch Statement**](Switch-Statement.md)

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

### Postfix Operation:

    + ── [Expression] ── postfixOperator ── +

### Function Call:



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







