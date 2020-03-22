
# Control Flow

## If Statements:

The condition needs to be a [Boolean](Types/Boolean.md) value, otherwise you'll get a *runtime error*.
Syntax is similar to *c++*, you can omit the curly braces if there's only one statement to evaluate.

``` swift

if (a == 0) {
    a += 1;
} else a = 8;

```

## Loops:

The condition needs to be a [Boolean](Types/Boolean.md) value, otherwise you'll get a *runtime error*.
Syntax is similar to c++, you can omit the curly braces if there's only one statement to evaluate.

``` swift

while (condition) {
    /* < code > */
}

do {
    /* < code > */
} while (condition);

until (condition) {
    /* < code > */
}

repeat {
    /* < code > */
} until (condition);

loop {
    /* < code > */
    if (condition) break;
}

for (Integer i = 0; i < 10; i += 1) {
    /* < code > */
    if (condition) continue;
    /* < code > */
}

```
