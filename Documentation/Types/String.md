
# String Type

The `String` type can be used for the
manipulation of sequences of characters.

## Usage

``` swift
var x: String = "Hello";
var y: String = "My favourite number is " + 5;
```

## Virtual Class Prototype

``` swift
class String {

	@secure var length: Integer = 0;

	@public proc append(element: Any);

	@public func contains(element: Character): Boolean;
	@public func contains(sequence: String): Boolean;

	@public proc clear();

	@immune con empty: self = "";

	@public func ends(with: Character): Boolean;
	@public func ends(with: String): Boolean;

	@public proc insert(element: Any, at: Integer);

	@public func isEmpty(): Boolean;

	@public proc prepend(element: Any);

	@public proc remove(at: Integer);
	@public proc remove(from: Integer, to: Integer);

	@public proc reverse();

	@public func starts(with: Character): Boolean;
	@public func starts(with: String): Boolean;

	@public func subString(from: Integer, to: Integer = length): String;

	@public func toLower(): String;
	@public func toTitle(): String;
	@public func toUpper(): String;
	@public func toWrong(): String;

}
```

### @secure Integer length = 0;

Read-only public property that contains the
total number of characters in the String.

``` swift
var length = "LONG STRING".length(); // length <- 11.
```

### @public proc append(element: Any);

Appends an object's string representation
to the end of the String.

**element**: The element to append.

``` swift
var s = "Hello";
s.append(" There!"; // s <- "Hello There!".
```

### @public func contains(element: Character): Boolean;

Checks if the Character is present in the current String.

**element**: The Character to look for.

``` swift
if ("1234".contains('3')) {
	/* Instructions */
}
```

### @public func contains(sequence: String): Boolean;

Checks if the sequence of characters is present
in the current String.

**sequence**: The sequence to look for.

``` swift
if ("1234".contains("23")) {
	/* Instructions */
}
```

### @public proc clear();

Sets the String to be empty (`""`).

``` swift
var s = "I'm full!";
s.empty(); // s <- "".
```

### @public func ends(with: Character): Boolean;

Checks if the String ends with the specified Character.\
If the String is empty it returns `false`.

**with**: The Character to look for.

``` swift
var s = "The pen is on the table!";
if (s.ends('!')) {
	/* Instructions */
}
```

### @public func ends(with: String): Boolean;

Checks if the String ends with the specified String.

**with**: The String sequence to look for.

``` swift
if ("1234".ends("34")) {
	/* Instructions */
}
```

### @public proc insert(element: Any, at: Integer);

Inserts the element's String representation
at the specified position.

**element**: The element to insert.\
**at**: The specified position.

``` swift
var s = "I U";
s.insert("<3 ", 2); // s <- "I <3 U".
```

### @public func isEmpty(): Boolean;

Check if the String is empty.

``` swift
var s = "I <3 U";
var heLovesMe = !s.isEmpty();
if (heLovesMe) {
	s += " 2"; // s <- "I <3 U 2".
}
```

### @public proc prepend(element: Any);

Prepends an object's String representation
to the start of the String.

**element**: The element to prepend.

``` swift
var s = "There!";
s.prepend("Hello "; // s <- "Hello There!".
```

### @public proc remove(at: Integer);

Removes the Character at the specified position.\
If the specified position is invalid it throws an
index out of range exception.

**at**: The specified position.

``` swift
var s = "I don't need a semicolon;";
s.remove(s.length - 1); // s <- "I don't need a semicolon".
```

### @public proc remove(from: Integer, to: Integer);

Removes the sequence of characters at the specified range.\
If the specified range is invalid it throws an
index out of range exception on one of the parameters.

**from**: The start position (included).\
**to**: The end position (included).

``` swift
var s = "Hello terrible friend!";
s.remove(6, 14); // s <- "Hello friend!".
```

### @public proc reverse();

Reverses the String, in place.

``` swift
var s = "olleH";
s.reverse(); // s <- "Hello".
```

### @public func starts(with: Character): Boolean;

Checks if the String starts with the specified Character.\
If the String is empty it returns `false`.

**with**: The Character to look for.

``` swift
var s = "The pen is on the table.";
if (s.starts('T')) {
	/* Instructions */
}
```

### @public func starts(with: String): Boolean;

Checks if the String starts with the specified String.

**with**: The String sequence to look for.

``` swift
if ("1234".starts("12")) {
	/* Instructions */
}
```

### @public func subString(from: Integer, to: Integer = length): String;

Returns the sequence of characters at the specified range.\
If the specified range is invalid it throws an
index out of range exception on one of the parameters.\
If the upper bound is not specified it returns a substring from
the lower bound to the end of the String.

**from**: The start position (included).\
**to**: The end position (included).

``` swift
var s = "Hello terrible friend!";
var howIsMyFriend = s.subString(6, 13); // s <- "terrible".
```

### @public func toLower(): String;

Returns the String in lowercase.

``` swift
var s = "UPPERCASE";
s = s.toLower(); // s <- "uppercase".
```

### @public func toTitle(): String;

Returns the String in titlecase.

``` swift
var s = "very big title";
s = s.toTitle(); // s <- "Very Big Title".
```

### @public func toUpper(): String;

Returns the String in uppercase.

``` swift
var s = "lowercase";
s = s.toUpper(); // s <- "LOWERCASE".
```

### @public func toWrong(): String;

Returns the String in wrongcase.\
It swaps the cases along the way.

``` swift
var s = "lowerCase";
s = s.toWrong(); // s <- "LOWERcASE".
```

### @immune String empty = "";

Read-only static property representing the empty String.

``` swift
var empty = String::empty; // empty <- "".
```
