# C++ Python Interpreter

A custom, lightweight Python syntax interpreter written from scratch in C++.
This project demonstrates the core concepts of compiler design: lexical analysis (tokenization), parsing (AST generation), and evaluation.

## Features

* **Custom Lexer & Parser**: Built from the ground up without external libraries.
* **Variable Management**: Support for variable declaration and assignment.
* **Arithmetic Operations**: Handles basic math expressions (`+`, `-`, `*`, `/`).
* **Control Flow**: Support for basic loops and conditionals (*Note: update this based on what your code actually supports*).
* **C++20**: Utilizes modern C++ standards for efficient memory management and performance.

## Build & Run

### Prerequisites
* A C++ compiler that supports C++20 (e.g., GCC 10+, Clang 10+, MSVC).

### Compilation
Open your terminal in the project folder and run the following command.
**Note:** Make sure to include all your `.cpp` files (e.g., `lexer.cpp`, `parser.cpp`, etc.).

```bash
g++ -std=c++20 main.cpp EvaluationVisitor.cpp Lexer.cpp Parser.cpp SymbolTable.cpp Syntax.cpp Token.cpp -o interpreter

```
### Usage
* **Running a Script
1 To execute a Python script, simply pass the file path as an argument to the executable.

- On Windows: PowerShell
```bash
.\interpreter.exe myscript.py
```
- On Linux / macOS: Bash
```bash
./interpreter myscript.py
```
2. Supported Syntax Examples
The interpreter currently supports a subset of Python syntax. Here is what you can write in your .py files:

- Variables & Math:
```bash
Python

x = 10
y = 5
result = (x + y) * 2
print(result)
Conditions (If/Else): (Remove this block if you haven't implemented 'if' yet)

Python

val = 10
if val > 5:
    print(1)
else:
    print(0)
```