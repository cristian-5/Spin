
# Type

A **Type** holds information about the kind of object.
A **customType** is a type defined by the programmer in the code.

### Syntax:

    + ──╮─ basicType ──╭─ +
        ╰─ customType ─╯

### Tokens used in this Rail (Regexes):

- **basicType** - Defined as `Integer|Real|String|Bool|Character|Colour|Complex|Imaginary|Byte`
- **customType** - Defined as `[A-Za-z_][A-Za-z0-9_]*`
