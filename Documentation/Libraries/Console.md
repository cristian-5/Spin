
# Console Library

The Console library provides support for input and output
operations, as well as colour and exit codes management.

**Information**: In order to use it you need to import it.

**Warning**: Colours are only supported on ANSI compatible terminals.

## Definition

``` swift

class Console {

	@shared proc write(object: Any...);
	@shared proc writeLine(object: Any...);
	@shared func read(object: Any...) -> String;
	@shared func readLine(object: Any...) -> String;

	@shared proc setBackground(r: Byte, g: Byte, b: Byte);
	@shared proc setBackground(c: Byte);
	@shared proc setForeground(r: Byte, g: Byte, b: Byte);
	@shared proc setForeground(c: Byte);

	@shared proc reset();
	@shared proc clean();

}

```

## Usage

``` swift
import Console;
```

### @shared proc write(object: Any...);

Writes object to the output buffer, without
ending the current line.

**object**: Can be any type of object.

``` swift
Console::write("Hello ", "world!");
```

### @shared proc writeLine(object: Any...);

Writes object to the output buffer,
ending the current line.

**object**: Can be any type of object.

``` swift
Console::writeLine("Hello world!");
```

### @shared func read(object: Any...) -> String;

Writes object to the output buffer, without
ending the current line, waits for user input,
returns the corresponding string.

**object**: Can be any type of object.

``` swift
String name = Console::read("Insert your name: ");
```

### @shared func readLine(object: Any...) -> String;

Writes object to the output buffer,
ending the current line, waits for user input,
returns the corresponding string.

**object**: Can be any type of object.

``` swift
String name = Console::readLine("Insert your name on the next line.");
```

### @shared proc setBackground(r: Byte, g: Byte, b: Byte);

Changes the output background colour.

**r**, **g**, **b**: The selected colour in RGB format.

``` swift
Console::setBackground(0x00, 0x00, 0x00); // Black.
```

### @shared proc setBackground(c: Byte);

Changes the output background colour.

**c**: The selected colour.

``` swift
Console::setBackground(83); // Greenish.
```

### @shared proc setForeground(r: Byte, g: Byte, b: Byte);

Changes the output foreground colour.

**r**, **g**, **b**: The selected colour in RGB format.

``` swift
Console::setForeground(0xFF, 0xFF, 0xFF); // White.
Console::setForeground(255, 0, 0);        // Red.
```

### @shared proc setForeground(c: Byte);

Changes the output foreground colour.

**c**: The selected colour.

``` swift
Console::setForeground(83); // Greenish.
```

### @shared proc reset();

Resets the buffer colours to the default settings.

``` swift
Console::reset();
```

### @shared proc clean();

Resets the colours and cleans the whole buffer.

``` swift
Console::clean();
```
