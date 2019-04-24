
# Stack Functions

### Declaration

Functions in Stack are defined with the keyword `func`
followed by an identifier, parameters enclosed in round
brackets, the return type and the block of code.

Parameters are separated with a comma and are defined
with an identifier followed by colon and the type.

```swift
func addition(a: Int32, b: Int32) -> Int32 {
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
proc increment(val: ref Int32) {
    a++;
}
```

## Call

To call a Stack function or procedure you must use
the identifier followed by evenual parameters
enclosed in round brackets.

```swift
Int32 x = addition(a: 5, b: 6);
increment(val: x);
```

To call a Stack function without specifying the
argument lables, you must define them with an
underscore.

```swift
proc decrement(_ a: ref Int32) {
    a--;
}

Int32 x = 5;
decrement(x);
```

To call a Stack function with an argument lable
and use another name instead of the lable used,
you can define the name after the lable.

```swift
func incrementedVersion(of value: Int32) -> Int32 {
    return value + 1;
}

Int32 x = 5;
x = incrementedVersion(of: x);
```
