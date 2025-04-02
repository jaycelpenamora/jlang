How to use:<br>
create executable first<br>
example 1: 
```
g++ -o jlang main.cpp lexer.cpp parser.cpp codegen.cpp interpreter.h 
```
then do ./jlang filename<br>
example 2: 

```
./jlang test.jlang
```
but you can just clone everything and run example 2.

How It Works <br>

    Lexer (Tokenizer):

        The lexer reads the raw source code and splits it into tokens, which represent individual components of the program (like variables, operators, numbers, punctuation, etc.).
<br>
    Parser:

        The parser takes these tokens and constructs an Abstract Syntax Tree (AST). The AST represents the structure of the program, breaking down expressions, assignments, and other statements into hierarchical nodes.
<br>
    Interpreter:

        The interpreter traverses the AST and executes the program by evaluating expressions, assigning variables, and printing output.
<br>

It supports: <br>
- Arithmetic operations (addition, subtraction, multiplication, division) <br>
- Variables Assignment <br>
- Print Statement <br>
