
# Literal

A **Literal Expression** is a single instruction that
encapsulates a literal object.

### Syntax:

    + ─╮── imaginary ──╭─ +
       |               |
       ╰── real ───────╯
       ╰── integer ────╯
       |               |
       ╰── string ─────╯
       ╰── character ──╯
       |               |
       ╰── colour ─────╯
       |               |
       ╰── bool ───────╯
       |               |
       ╰── null ───────╯

### Tokens used in this Rail (Regexes):

- **imaginary** - Defined as `[0-9]+(?:\.[0-9]+(?:[eE][-]?[0-9]+)?)?i`
- **real** - Defined as `[0-9]+\.[0-9]+(?:[eE][-]?[0-9]+)?`
- **integer** - Defined as `(?:0[x][0-9A-Fa-f]+)|(?:0b[01]+)|(?:0o[0-7]+)|(?:0d[0-9]+)|(?:[0-9]+)`
- **string** - Defined as `"(?:[^\\"]|\\["\\0abfnrtv]|\\0x[0-9A-Fa-f]{2})*"`
- **character** - Defined as `'(?:[^\\]|\\0x[0-9A-Fa-f]{2}|\\['\\0abfnrtv])'`
- **colour** - Defined as `#[A-Fa-f0-9]{6}(?:[A-Fa-f0-9][A-Fa-f0-9])?|#[A-Fa-f0-9]{3,4}`
- **bool** - Defined as `true|false`
- **null** - Defined as `null`
