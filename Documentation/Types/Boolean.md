
# Boolean Type

The `Boolean` type can be used for logical expression and
it's the only type that can be used to determine
the outcome of a logical statement.

Unlike *c++* the type can't store integer values.

In a file serialization to avoid read-write
mistakes, the value `true` will be written
as `0xFF` and the `false` value will be `0x00`.

## Usage Examples

``` java
Boolean x = false;
Boolean y = 5 > 0; // y <- true.
```

## Virtual Class Prototype

``` swift
class Boolean {

	@shared func == (Boolean, Boolean) -> Boolean;
	@shared func != (Boolean, Boolean) -> Boolean;

	@public proc toggle();

	@shared func ! (Boolean) -> Boolean;

	@shared func || (Boolean, Boolean) -> Boolean;
	@shared func && (Boolean, Boolean) -> Boolean;

	@shared func from(Boolean) -> Boolean;

	@public func toString() -> String;

	@immune String description = "Boolean";

}
```
