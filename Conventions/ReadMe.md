
# Conventions

This section helps developers organise their code following
the same code conventions.

## File Conventions

File names should be in **TitleCase** and can be contracted
if the name is made of compound words (Eg: AbstractSyntaxTree -> AST).

There's only one .cpp file and that's the Main.
Libraries are defined under namespaces in .hpp files.

Group the header files together using folders.
Folder names should be in TitleCase as well.

Underscores are always forbidden in file names, or code declarations.

## Code Conventions

Always use **Guard Includes** for libraries.

Always include the License Header inside the file following
this pattern:

``` cpp
/*!
 *
 *    + --------------------------------------- +
 *    |  FileName.extension                     |
 *    |                                         |
 *    |                File Name                |
 *    |                                         |
 *    |  Created by Author.                     |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
 */
```

Be sure to center the name in the box.

Always leave a *blank line* at the beginning and end of a file.

Always use **spaces** to separate *operators*, *commas*, *parameters*
and *keywords* (with their brackets). Also separate `&` and `*`.

Always use the aliases defined instead of the **Type Keywords**.

Try to wrap code when necessary and don't use context braces
for one statement (`if (true) { i += 1; }` -> `if (true) i += 1;`).

Function identifiers should be camelCased and Objects TitleCased.

Never use ++ or -- since it slows down the execution.
Use instead += 1 or -= 1.

## Documentation Conventions

Always write documentation (if needed) for your code
in the multiline doxygen style.
