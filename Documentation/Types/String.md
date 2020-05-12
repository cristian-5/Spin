
# String Type

The `String` type can be used for the
manipulation of sequences of characters.

## Usage

``` java
String x = "Hello";
String y = "My favourite number is " + 5;
```

## Virtual Class Prototype

``` swift
class String {

	@secure Integer length = 0;

	@public proc append(element: Any);

	@public func contains(element: Character) -> Boolean;
	@public func contains(sequence: String) -> Boolean;

	@public proc clear();

	@public func ends(with: Character) -> Boolean;
	@public func ends(with: String) -> Boolean;

	@public proc insert(element: Any, at: Integer);

	@public func isEmpty() -> Boolean;

	@public proc prepend(element: Any);

	@public proc remove(at: Integer);
	@public proc remove(from: Integer, to: Integer);

	@public proc reverse();

	@public func starts(with: Character) -> Boolean;
	@public func starts(with: String) -> Boolean;

	@public func subString(from: Integer, to: Integer = length) -> String;

	@public func toLower() -> String;
	@public func toTitle() -> String;
	@public func toUpper() -> String;
	@public func toWrong() -> String;

	@immune String empty = "";

	@shared func from(Any) -> String;

	@define func + (String, Any) -> String;
	@define func + (Any, String) -> String;
	@define proc += (Any);
	@define func [] (Integer) -> Character;

	@define func toString() -> String;

}
```

### @secure Integer length = 0;

Read-only public property that contains the
total number of characters in the String.

``` java
Integer length = "LONG STRING".length(); // length <- 11.
```

### @public proc append(element: Any);

Appends an object's string representation
to the end of the String.

**element**: The element to append.

``` java
String s = "Hello";
s.append(" There!"; // s <- "Hello There!".
```

### @public func contains(element: Character) -> Boolean;

Checks if the Character is present in the current String.

**element**: The Character to look for.

``` java
if ("1234".contains('3')) {
	/* Instructions */
}
```

### @public func contains(sequence: String) -> Boolean;

Checks if the sequence of characters is present
in the current String.

**sequence**: The sequence to look for.

``` java
if ("1234".contains("23")) {
	/* Instructions */
}
```

### @public proc clear();

Sets the String to be empty (`""`).

``` java
String s = "I'm full!";
s.empty(); // s <- "".
```

### @public func ends(with: Character) -> Boolean;

Checks if the String ends with the specified Character.\
If the String is empty it returns `false`.

**with**: The Character to look for.

``` java
String s = "The pen is on the table!";
if (s.ends('!')) {
	/* Instructions */
}
```

### @public func ends(with: String) -> Boolean;

Checks if the String ends with the specified String.

**with**: The String sequence to look for.

``` java
if ("1234".ends("34")) {
	/* Instructions */
}
```

### @public proc insert(element: Any, at: Integer);

Inserts the element's String representation
at the specified position.

**element**: The element to insert.\
**at**: The specified position.

``` java
String s = "I U";
s.insert("<3 ", 2); // s <- "I <3 U".
```

### @public func isEmpty() -> Boolean;

Check if the String is empty.

``` java
String s = "I <3 U";
Boolean heLovesMe = !s.isEmpty();
if (heLovesMe) {
	s += " 2"; // s <- "I <3 U 2".
}
```

### @public proc prepend(element: Any);

Prepends an object's String representation
to the start of the String.

**element**: The element to prepend.

``` java
String s = "There!";
s.prepend("Hello "; // s <- "Hello There!".
```

### @public proc remove(at: Integer);

Removes the Character at the specified position.\
If the specified position is invalid it throws an
index out of range exception.

**at**: The specified position.

``` java
String s = "I don't need a semicolon;";
s.remove(s.length - 1); // s <- "I don't need a semicolon".
```

### @public proc remove(from: Integer, to: Integer);

Removes the sequence of characters at the specified range.\
If the specified range is invalid it throws an
index out of range exception on one of the parameters.

**from**: The start position (included).\
**to**: The end position (included).

``` java
String s = "Hello terrible friend!";
s.remove(6, 14); // s <- "Hello friend!".
```

### @public proc reverse();

Reverses the String, in place.

``` java
String s = "olleH";
s.reverse(); // s <- "Hello".
```

### @public func starts(with: Character) -> Boolean;

Checks if the String starts with the specified Character.\
If the String is empty it returns `false`.

**with**: The Character to look for.

``` java
String s = "The pen is on the table.";
if (s.starts('T')) {
	/* Instructions */
}
```

### @public func starts(with: String) -> Boolean;

Checks if the String starts with the specified String.

**with**: The String sequence to look for.

``` java
if ("1234".starts("12")) {
	/* Instructions */
}
```

### @public func subString(from: Integer, to: Integer = length) -> String;

Returns the sequence of characters at the specified range.\
If the specified range is invalid it throws an
index out of range exception on one of the parameters.\
If the upper bound is not specified it returns a substring from
the lower bound to the end of the String.

**from**: The start position (included).\
**to**: The end position (included).

``` java
String s = "Hello terrible friend!";
String howIsMyFriend = s.subString(6, 13); // s <- "terrible".
```

### @public func toLower() -> String;

Returns the String in lowercase.

``` java
String s = "UPPERCASE";
s = s.toLower(); // s <- "uppercase".
```

### @public func toTitle() -> String;

Returns the String in titlecase.

``` java
String s = "very big title";
s = s.toTitle(); // s <- "Very Big Title".
```

### @public func toUpper() -> String;

Returns the String in uppercase.

``` java
String s = "lowercase";
s = s.toUpper(); // s <- "LOWERCASE".
```

### @public func toWrong() -> String;

Returns the String in wrongcase.\
It swaps the cases along the way.

``` java
String s = "lowerCase";
s = s.toWrong(); // s <- "LOWERcASE".
```

### @immune String empty = "";

Read-only static property representing the empty String.

``` java
String empty = String::empty; // empty <- "".
```

### @shared func from(Any) -> String;

Forces a cast from an expression.

``` java
String fromInteger = String::from(10);
String fromBoolean = String::from(false);
```

## Operators

> Comparison Operators like `==` and `!=`,
> where the operands share the same type,
> are always supported by every Basic Type.

The **String** type allows the use of the following operators:

### @define func + (String, Any) -> String;

Infix binary operator that returns the string resulting
from the concatenation of the string values of the operands.

``` java
String x = "Agent " + 707; // x <- "Agent 707".
```

### @define func + (Any, String) -> String;

Infix binary operator that returns the string resulting
from the concatenation of the string values of the operands.

``` java
String x = 3.5 + " seconds"; // x <- "3.5 seconds".
```

### @define proc += (Any);

Infix binary operator that appends the operand to the String.

``` java
String x = "Hello";
x += " There"; // x <- "Hello There".
```

### @define func [] (Integer) -> Character;

Subscript unary operator that returns the Character
found in the specified position.\
If the position is invalid it throws an index out
of range exception.

``` java
Character c = "ABCD"[2]; // x <- 'C'.
```

## Overrides

### @define func toString() -> String;

Gets the string representation of the value.

``` java
import Console;
String value = "value";
Console::writeLine(value.toString()); // prints "value".
```
