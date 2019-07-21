
# Stack Types

Stack has a set of native types like C or C++.
Those types are actually virtual classes that
contain methods and properties.

### Native Integer types

In Stack the programmer must always specify the number
of bits reserved for an integer value according to the
following table.

|       Type | Size     | Description      |
|-----------:|----------|------------------|
|   **Int8** |  1 Byte  | Signed Integer   |
|  **UInt8** |  1 Byte  | Unsigned Integer |
|  **Int16** |  2 Bytes | Signed Integer   |
| **UInt16** |  2 Bytes | Unsigned Integer |
|  **Int32** |  4 Bytes | Signed Integer   |
| **UInt32** |  4 Bytes | Unsigned Integer |
|  **Int64** |  8 Bytes | Signed Integer   |
| **UInt64** |  8 Bytes | Unsigned Integer |
| **Colour** |  4 Bytes | Unsigned Integer |

### Types internally seen as Integers

|       Type | Size     | Description      |        Aliases       |
|-----------:|----------|------------------|:--------------------:|
|   **Bool** |  1 Byte  | Boolean Value    | *Boolean, Bit, Flag* |
|   **Char** |  1 Byte  | Character        |      *Character*     |

### Real Number Types

|          Type | Size     | Description           |        Aliases       |
|--------------:|----------|-----------------------|:--------------------:|
|     **Float** |  4 Bytes | Floating Point        |     *Floating*       |
|    **Double** |  8 Bytes | Double Precision      |                      |
|      **Real** | 12 Bytes | Real Number           |                      |
| **Imaginary** | 12 Bytes | Imaginary Real Number |                      |

### Complex Type

Every `Complex` Number ins Stack is made of a Real part and
an Imaginary one.

|        Type | Size     | Description      |
|------------:|----------|------------------|
| **Complex** | 24 Bytes | Complex Number   |

### String Type

Every `String` in Stack is a list of characters and contains
methods and properties as well as attributes meant for string
manipulation.

|       Type | Size     | Description      |
|-----------:|----------|------------------|
| **String** | **N**    | ASCII String     |

### Isomorphic Types

The `Isomorphic` type is a special type that identifies a special
pointer to an object or a class which can mutate its type based
on the current situation. For example we can define an Isomorphic
variable, give it a value that does not exceed 8 bits and then
convert that value in a 64 bits Integer or in a Real value, not
having to worry about the cast, which will be automatic.
If the value can't be casted then it mutates to the default value.

|       Type | Size     | Description      |        Aliases       |
|-----------:|----------|------------------|:--------------------:|
|    **Iso** | **N**    | Class Pointer    |      *Isomorphic*    |
