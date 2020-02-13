
# Spin Types

Spin has a set of native types like **c** or **c++**.
Those types are actually virtual classes that
contain methods and properties.

### Native Integer types

|          Type | Size     | Description      |
|--------------:|----------|:----------------:|
|      **Byte** |  1 Byte  | Unsigned Integer |
|   **Integer** |  8 Bytes | Signed Integer   |

### Types seen internally as Integers

|          Type | Size     | Description      |
|--------------:|----------|:----------------:|
|      **Bool** |  1 Byte  | Bool Value       |
| **Character** |  1 Byte  | Character        |

### Real Number Types

|          Type | Size     | Description      |
|--------------:|----------|:----------------:|
|      **Real** |  8 Bytes | Real Number      |
| **Imaginary** |  8 Bytes | Imaginary Number |

### Complex Type

Every `Complex` Number ins Spin is made of a Real part and
an Imaginary one.

|          Type | Size     | Description      |
|--------------:|----------|:----------------:|
|   **Complex** | 16 Bytes | Complex Number   |

### String Type

Every `String` in Spin is a dynamic array of
characters and contains methods and properties as
well as attributes meant for string manipulation.

|          Type | Size     | Description      |
|--------------:|----------|:----------------:|
|    **String** | **N**    | ASCII String     |

### Quantum Types

Quantum types include Vectors, Matrices and QBits.

|          Type | Size     | Description      |
|--------------:|----------|:----------------:|
|    **Vector** | **16N**  | Complex Vector   |
|    **Matrix** | **16NN** | Complex Matrix   |
