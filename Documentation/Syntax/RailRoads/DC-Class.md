
# Class

Holds **Class Declarations**.

### Syntax:

    + class ── identifier ── { ──╮─────────────── > ─────────────╭─ } ── +
                                 ╰─╭── [Function Declaration] ─╮─╯
                                 | ╰───────────── < ───────────╯ |
                                 ╰─╭─ [Procedure Declaration] ─╮─╯
                                 | ╰───────────── < ───────────╯ |
                                 ╰─╭── [Property Declaration] ─╮─╯
                                 | ╰───────────── < ───────────╯ |
                                 ╰──────── [Constructor] ────────╯
                                 ╰──────── [Destructor] ─────────╯

### Tokens used in this Rail (Regexes):

- **class** - Defined as `class`
- **identifier** - Defined as `[A-Za-z_][A-Za-z0-9_]*`

### Declarations used in this Rail:

- [**Procedure Declaration**](DC-Procedure.md)
- [**Function Declaration**](DC-Function.md)
- [**Property Declaration**](DC-Property.md)
- [**Constructor**](DC-Constructor.md)
- [**Destructor**](DC-Destructor.md)
