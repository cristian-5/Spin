
# Short Circuit AND

A **Short Circuit AND Expression** doesn't need
to evaluate right and left side of itself to work.

### Syntax:

    + ── [Equality] ─╮────────────── > ──────────────╭─ +
                     |  ╭─────────── < ───────────╮  |
                     ╰──╰─── and ── [Equality] ───╯──╯

### Tokens used in this Rail (Regexes):

- **and** - Defined as `&&`

### Declarations used in this Rail:

- [**Equality**](EX-Equality.md)
