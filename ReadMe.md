
# Stack Language

Stack Programming Language

High level programming language for quantum
computing based on C++, Swift C#.
The documentation contains the language syntax specification.

- [**Syntax Specification**](Documentation/Syntax/)

## Compilation

This code is written in **c++17** so don't try to build
it using an *older version*.

The Compiler must be compiled using *g++*,
*clang++* or *Visual Studio*.

We recommend using clang++ on **macOS** and Visual
Studio developer console in **Windows**.

### macOS Compilation

From the *Terminal* app change the current directory to
the Stack project directory.
Execute the following to compile using **clang++**:

    cd Compiler
	clang++ -o Main Main.cpp -std=c++17

This creates a `Main` file in the same folder;
drop it in the console to execute it.

### Windows Compilation

Open up Visual Studio Developer Console and change
the working directory to the Stack project directory.
Execute the following to compile using **cl**:

    cd Compiler
    cl /EHsc Main.cpp

This creates a `Main.obj` and a `Main.exe` file in the
same folder; double click is sufficient to run the code.


