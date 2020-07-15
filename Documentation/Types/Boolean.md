
# Boolean Type

The `Boolean` type can be used for logical expression and
it's the only type that can be used to determine
the outcome of a logical statement.

Unlike **c++** the type can't store integer values.

In file serialisations, to avoid read-write
mistakes, the value `true` will be written
as `0xFF` while `false` will be `0x00`.

## Usage

``` java
Boolean x = false;
Boolean y = 5 > 0; // y <- true.
```

## Virtual Class Prototype

``` swift
class Boolean {

	@public proc toggle();

	@shared func from(Boolean) -> Boolean;

	@shared func ! (Boolean) -> Boolean;
	@shared func || (Boolean, Boolean) -> Boolean;
	@shared func && (Boolean, Boolean) -> Boolean;

	@define func toString() -> String;

}
```

### @public proc toggle();

Toggles the value.

``` java
Boolean x = false.toggle(); // x <- true.
```

### @shared func from(Boolean) -> Boolean;

Forces a cast from an expression.

``` java
Boolean fromExpression = Boolean::from(x != 10);
```

## Operators

> Comparison Operators like `==` and `!=`,
> where the operands share the same type,
> are always supported by every Basic Type.

The **Boolean** type allows the use of the following operators:

### @define func ! (Boolean) -> Boolean;

Prefix operator that returns the inverted (toggled) value.

``` java
Boolean x = !false; // x <- true.
```

### @define func || (Boolean, Boolean) -> Boolean;

> This operator will always short circuit when used twice (`||`).\
> To avoid this behaviour use a single `|` instead of two.

Infix binary operator that performs the logic `OR`
between the two operands and returns the result.

``` java
Boolean x = false || false; // x <- false.
```

### @define func && (Boolean, Boolean) -> Boolean;

> This operator will always short circuit when used twice (`&&`).\
> To avoid this behaviour use a single `&` instead of two.

Infix binary operator that performs the logic `AND`
between the two operands and returns the result.

``` java
Boolean x = true && false; // x <- false.
```

## Overrides

### @define func toString() -> String;

Gets the string representation of the value.\
It returns either `"true"` or `"false"`.

``` java
Boolean x = true;
String v1 = x;            // v1 <- "true".
x.toggle();
String v2 = x.toString(); // v2 <- "false".
```