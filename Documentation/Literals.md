
# Literals

Literals are special tokens in the source code treated as values.

**Attention**: Scientific notation is only supported with floating
point numbers, using *lowercase* `e`. Hexadecimal numbers must be
in *UPPERCASE* and must always be preceded by the `0x` prefix.

Spin offers a wide variety of literals:

|       Literal | Description      | Examples                             |
|--------------:|------------------|:------------------------------------:|
| **Imaginary** | Imaginary Number | `5i` `5.4i` `54.4e-4i`               |
|      **Real** | Real Number      | `3.673` `7.9e-8`                     |
|   **Integer** | Integer Number   | `67` `0x5F` `0d16` `0b1010` `0o76`   |
|    **String** | String           | `"hello world"` `"\t tab"`           |
| **Character** | Character        | `'a'` `'A'` `'$'` `'\n'` `'\0xB2'`   |
|      **Bool** | Bool Value       | `true` `false`                       |
|    **Vector** | Quantum Basis    | `<0|` `<1|`                          |
|    **Vector** | Quantum Basis    | `|0>` `|1>`                          |

The types associated with this table can be
found id the [Types](Types.md) section.
