
# Stack Types

Stack has a set of native types like C or C++.
Those types are actually virtual classes that
contain methods and properties.

### Native Integer types

|       Type | Size     | Description      |
|-----------:|----------|:----------------:|
|   **Byte** |  1 Byte  | Unsigned Integer |
|  **Int64** |  8 Bytes | Signed Integer   |

### Types seen internally as Integers

|       Type | Size     | Description      |
|-----------:|----------|------------------|
|   **Bool** |  1 Byte  | Boolean Value    |
|   **Char** |  1 Byte  | Character        |
| **Colour** |  4 Bytes | Unsigned Integer |

### Real Number Types

|          Type | Size     | Description           |
|--------------:|----------|-----------------------|
|      **Real** | 12 Bytes | Real Number           |
| **Imaginary** | 12 Bytes | Imaginary Real Number |

### Complex Type

Every `Complex` Number ins Stack is made of a Real part and
an Imaginary one.

|        Type | Size     | Description      |
|------------:|----------|------------------|
| **Complex** | 24 Bytes | Complex Number   |

### String Type

Every `String` in Stack is a dynamic array of
characters and contains methods and properties as
well as attributes meant for string manipulation.

|       Type | Size     | Description      |
|-----------:|----------|------------------|
| **String** | **N**    | ASCII String     |
