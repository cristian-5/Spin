
# Stack Syntax RailRoad

Here's a **railway** implementation of statements' syntax.

- **Tokens** are defined as characters sequences.
- **References** to other railways are enclosed in square brackets.
- **Symbols** are defined as isolated characters.
- **Loops** (self references) are defined as loops and arrows.

### No Operation:

    + ─╮─── > ───╭─ ; ── +
       ╰── nop ──╯

### Statement:

    + ─╮── > ── [Code Block] ─────────────── > ──╭─ +
       |                                         |
       ╰── > ── [If Statement] ───────────── > ──╯
       ╰── > ── [Switch Statement] ───────── > ──╯
       |                                         |
       ╰── > ── [Do While Statement] ─────── > ──╯
       ╰── > ── [Repeat Until Statement] ─── > ──╯
       ╰── > ── [Loop Statement] ─────────── > ──╯
       |                                         |
       ╰── > ── [For Statement] ──────────── > ──╯
       ╰── > ── [For Each Statement] ─────── > ──╯
       |                                         |
       ╰── > ── [Try Catch Statement] ────── > ──╯
       ╰── > ── [Avoid Statement] ────────── > ──╯
       ╰── > ── [Throw Statement] ────────── > ──╯
       |                                         |
       ╰── > ── [Expression Statement] ───── > ──╯
       |                                         |
       ╰── > ── [Class Declaration] ──────── > ──╯
       ╰── > ── [Exception Declaration] ──── > ──╯
       ╰── > ── [Structure Declaration] ──── > ──╯
       ╰── > ── [Enumerator Declaration] ─── > ──╯
       ╰── > ── [Function Declaration] ───── > ──╯
       ╰── > ── [Variable Declaration] ───── > ──╯
       |                                         |
       ╰── > ── [No Operation] ───────────── > ──╯
       |                                         |
       ╰── > ── [Return Statement] ───────── > ──╯
       ╰── > ── [Break Statement] ────────── > ──╯
       ╰── > ── [Continue Statement] ─────── > ──╯

### Expression Statement:

    + ── [Expression] ── ; ── +

### Statements:

    + ─╭─ [Statement] ─╮─ +
       ╰────── < ──────╯

### Code Block:

    + ── { ─╮────── > ───────╭─ } ── +
            ╰─ [Statements] ─╯

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

### If Statement:

    + ──── if ──── ( ──── [Expression] ──── ) ────╮
    ╭── < ───────────────── < ─────────────── < ──╯
    ╰── [Statement] ──╮────────── > ──────────╭── +
                      ╰─ else ── [Statement] ─╯

### Switch Statement:

    + ──── switch ─── ( ── [Expression] ── ) ─── > ───╮
    ╭─── < ────────────────── < ──────────────── < ───╯
    ╰── { ─╭─ case ── [Literal] ─ : ─ [Statement] ─╮──╮
           ╰─────────────────── < ─────────────────╯  |
    ╭──────── < ─────────── < ───────────── < ────────╯
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







