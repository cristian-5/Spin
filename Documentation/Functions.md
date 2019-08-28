
# Stack Functions

### Declaration

Functions in Stack are defined with the keyword `func`
followed by an identifier, parameters enclosed in round
parenthesis, the return type and the block of code.

Parameters are separated with a comma and are defined
with an identifier followed by colon and the type.

```swift
func addition(a: Integer, b: Integer) -> Integer {
    return a + b;
}
```

# Stack Procedures

### Declaration

Procedures in Stack are defined with the keyword `proc`
followed by an identifier, parameters enclosed in round
brackets and the block of code.

Parameters are separated with a comma and are defined
with an identifier followed by colon and the type.

```swift
proc increment(val: ref Integer) {
    a += 1;
}
```

## Call

To call a Stack function or procedure you must use
the identifier followed by eventual parameters
enclosed in round brackets.

```swift
Integer x = addition(5, 6);
increment(x);
```

## Reference and Copy

Classes are always passed as reference in functions
and procedures but can be passed as a copy using the
`cpy` specifier after the argument label.

```swift
proc hello(val: cpy CustomClass) {
    // Do Stuff.
}
```

Basic Types are always passed as copies in functions
and procedures but can be passed as reference using the
`ref` specifier after the argument label.

```swift
proc hello(val: ref Integer) {
    val += 1;
}
```