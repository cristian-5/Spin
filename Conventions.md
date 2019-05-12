
# Conventions

This section helps developers organize their code following
the same code conventions.

## File Conventions

File names should be in **TitleCase** and can be contracted
if the name is made of compound words (Eg: SyntaxRule -> SRule).

There's only one .cpp file and that's the Main.
Libraries are defined under namespaces in .hpp files.

Group the header files together using folders.
Folder names should be in TitleCase as well.

Underscore are always forbidden in file names, or code declarations.

## Code Conventions

Always use **Guard Includes** for libraries.

Always include every file needed in a library, so that can be
eventually compiled alone for testing purposes.

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
for one statement (`if (true) { i++; }` -> `if (true) i++;`).

## Documentation Conventions

Always write documentation for your code in the multiline doxygen style.
