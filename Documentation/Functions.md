
# Spin Functions

### Declaration

Functions in Spin are defined with the keyword `func`
followed by an identifier, parameters enclosed in round
parenthesis, the return type and the block of code.

Parameters are separated with a comma and are defined
with an identifier followed by colon and the type.

``` swift
func addition(a: Integer, b: Integer) -> Integer {
    return a + b;
}
```

# Spin Procedures

### Declaration

Procedures in Spin are defined with the keyword `proc`
followed by an identifier, parameters enclosed in round
brackets and the block of code.

Parameters are separated with a comma and are defined
with an identifier followed by colon and the type.

``` swift
proc test(val: Integer) {
    print val + 1;
}
```

## Call

To call a Spin function or procedure you must use
the identifier followed by eventual parameters
enclosed in round brackets.

``` swift
// This prints 6:
test(5);
```

## Reference and Copy

Classes are always passed and returned by reference in functions
and procedures. You can use the assignment operator to make copies.
Basic Types are always passed by copy in functions and procedures.
