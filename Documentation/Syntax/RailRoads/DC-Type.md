
# Type

A **Type** holds information about the kind of object.
A **customType** is a type defined by the programmer in the code.

### Syntax:

    + ──╮────── basicType ──────╭─ +
        ╰────── customType ─────╯
        ╰─ [ ── basicType ── ] ─╯

### Tokens used in this Rail (Regexes):

- **basicType** - Defined as `Bool|Byte|Character|Colour|Complex|Imaginary|Integer|Real|String`
- **customType** - Defined as `[A-Za-z_][A-Za-z0-9_\-]*`
