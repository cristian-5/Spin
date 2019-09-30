
# Delete Statement

A **Delete Statement** deletes a variable and removes
its name from the lexical scope. You can reuse the
same name for a redefinition later. Deleting a class
also results in calling its destructor.

### Syntax:

    + ── delete ── identifier ── ; ── +

### Tokens used in this Rail (Regexes):

- **delete** - Defined as `delete`
- **identifier** - Defined as `[A-Za-z_][A-Za-z0-9_\-]*`
