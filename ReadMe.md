
# Spin Language

[![license](Badges/License.svg)](License)
[![version](Badges/Version.svg)](ReadMe.md)
[![language](Badges/Language.svg)](ReadMe.md)
[![contributions](Badges/Contributions.svg)](ReadMe.md)
[![donations](Badges/Donations.svg)](https://paypal.me/CristianAntonuccio)

**Spin Programming Language**

High level programming language for *research* in
**Quantum Computing** built in *c++*.

The **Virtual Machine** and the **Compiler** are
under development right now.
We aim to create a powerful language.

The quantum part of the language is not ready yet,
Dirac notation has been implemented in the lexer and
preprocessor, but the **VM** doesn't support it yet.

I'm speeding up the development process since this
will be my thesis.

## A taste of spin

**Attention!** These examples have only been conceptualised
and we are still implementing them in the **VM**.\
If you're looking for the Interpreter, contact me.

More examples can be found in the [Examples](Examples/) folder.

### Numeric Types

``` swift
var a: Integer = 3 + 2 * 7 - 5;
var b: Integer = 0b10; // binary
b = 0xAB24 * 0o547474; // hex * oct
var c: Real = 7.7;
var i: Imaginary = - 7i;
var d = 34.6 + 12i + c;
var e: Complex = a * b - (3.5 + 6i);
con pi = 3.14; // constant
```

### Minor Types

``` swift
var condition: Boolean = false;
var raw: Byte = 0xFF;
raw = ~raw; // bitwise inversion
var hello: String = "number " + '!';
var exclam: Character = hello[10];
```

### Console Library

Check out the full [**Console Documentation**](Documentation/Libraries/Console.md)

``` swift
import Console;

Console::writeLine("I'm using Console!");
```

The documentation contains the language syntax specification.

- [**Syntax Specification**](Documentation/)

## Code Contribution

**Attention!** This project is currently under development and
we only completed 30% of the total tasks. We are working on the
*Virtual Machine* and on the *Quantum System*.

**New Contributors are WELCOME**, so if you're a skilled dev
feel free to contact me for joining our team.

If you want to contribute you must follow these 
[Conventions](Conventions/) in order to keep the
file structure organised and the code clean.

## Donations

**Donations** are *welcome*! Offer me a *cup of coffee* since
I drink it *every day* just before working on this project.

[![donations](Badges/PayPal.svg)](https://paypal.me/CristianAntonuccio)

## Compilation

This code is written in **c++2a** so don't try to build
it using an *older version*.

The **VM** must be compiled using *clang++*.
We don't cover support for *gcc* / *g++* and we don't make
sure that our source will be successfully compiled with them.

We recommend using *clang++* on **macOS** and **Linux**.

The recommended version of *clang++* (**LLVM**) is 9.0.0.
