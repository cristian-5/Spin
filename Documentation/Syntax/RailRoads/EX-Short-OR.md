
# Short Circuit OR

A **Short Circuit OR Expression** doesn't need
to evaluate right and left side of itself to work.

### Syntax:

    + ── [Short AND] ─╮────────────── > ──────────────╭─ +
                      |  ╭─────────── < ───────────╮  |
                      ╰──╰── or ──── [Short AND] ──╯──╯

### Tokens used in this Rail (Regexes):

- **or** - Defined as `||`

### Declarations used in this Rail:

- [**Short AND**](EX-Short-AND.md)
