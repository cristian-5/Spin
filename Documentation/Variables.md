
# Variables

Variables represent storage locations. Every variable has a
type that determines what values can be stored in the variable.
Spin is a type-safe language, and the compiler guarantees that
values stored in variables are always of the appropriate type.

A variable doesn't need to be assigned before its value can
be obtained because every basic type has a default value.

## Variable categories

Spin defines several categories of variables: static
variables, instance variables, array elements, value
parameters, reference parameters and local variables.
The sections that follow describe each of these categories.

In the example

``` swift
import Console;

class A {

    @shared var x: Integer = 6;
    @hidden var y: Integer = 9;

    @create proc A() {
        Console::writeLine("Running Constructor");
    }

    @public proc subProgram(a: Real, b: Integer) {
        var i: Integer = 10;
        var c: Integer = a + b;
        c += i * a;
        self::x = c + self.y;
    }

}

A a = new A();
a.subProgram(3.0, 3);

```

`x` is a public static variable, `y` is a private instance variable,
`a` is a value parameter and `i` is a local variable.

### Static variables

A field declared with the `@static` or `@shared` modifier is called a
*static variable*. A static variable comes into existence
before execution of the static constructor for its containing
type, and ceases to exist when the associated application
domain ceases to exist.
The only difference between the two is that `@static` is private
while `@shared` is public.

The initial value of a static variable is the default value
of the variable's type.

For purposes of definite assignment checking, a static variable
is considered initially assigned.

### Instance variables

A field declared with the `@public`, `@secure` or `@hidden` modifier is called an *instance variable*.
The `@secure` modifier will allow read access to a field from outside of the declaration but
won't allow write permissions.

#### Instance variables in classes

An instance variable of a class comes into existence when a new
instance of that class is created, and ceases to exist when there
are no references to that instance and the instance's destructor
(if any) has executed.

The initial value of an instance variable of a class is the default
value of the variable's type.

For the purpose of definite assignment checking, an instance
variable of a class is considered initially assigned.

#### Instance variables in structures

An instance variable of a structure has exactly the same lifetime as the
structure variable to which it belongs. In other words, when a variable
of a structure type comes into existence or ceases to exist, so too do
the instance variables of the structure.

The initial assignment state of an instance variable of a structure is the
same as that of the containing structure variable.

### Array elements

The elements of an array come into existence when an array instance is
created, and cease to exist when there are no references to that array instance.

The initial value of each of the elements of an array is the
default value of the type of the array elements.

For the purpose of definite assignment checking, an array element
is considered initially assigned.

### Local variables

A *local variable* is declared by a local variable declaration,
which may occur in a *block*, a *for statement*, a
*switch statement* or a *specific catch clause* for a *try statement*.

The lifetime of a local variable is the portion of program
execution during which storage is guaranteed to be reserved
for it. This lifetime extends at least from entry into the
*block*, *for statement*, *switch statement*, or *specific catch clause*
with which it is associated, until execution ends in any way.
(Entering an enclosed *block* or calling a method suspends,
but does not end, execution of the current code.)
If the local variable is captured by an anonymous function,
its lifetime extends at least until the delegate or expression
tree created from the anonymous function, along with any other
objects that come to reference the captured variable, are
eligible for garbage collection.
