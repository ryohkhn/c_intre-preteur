# Projet

The goal of the project is to write a library of functions that implement operations on integers with an arbitrary number of digits.

The `-i` option of the program gives the name of a text file that contains a sequence of instructions to be executed by the interpreter. 
In the `-o` option you pass the name of the file where `calc_unbounded_int` will write the results.

We assume that both options are optional: if the `-i` option is absent `calc_unbounded_int` will read the instructions from the standard stdin input stream and if the `-o` option is absent the program will write the results in the `stdout` stream.

The file read by the interpreter contains three types of instructions, distributed on separate lines separate lines:

`variable = integer_or_variable op integer_or_variable.`

`integer_or_variable` is either an integer or a variable. 
`op` is one of three operations: `*, +, -.` 

The interpreter must perform the operation, the result of the operation becomes the value of the variable.

`variable = integer`

integer becomes the new value of the variable.

`print variable`

The interpreter will write a line in the output file as `variable = value` where value is the value of variable.

We will assume that the initial value of each variable is 0, in particular print a on a non-initialized variable a will write `a = 0` in the output file.

It is also assumed that:

- each line contains at most one instruction,

- it is possible to have empty lines (without instruction),

- there are no parenthesis,

- there are spaces (at least one on each side) around each operation `* , + , -`

- positive integers can be written either as 23432 or as +98676565 with `+` at the beginning followed immediately by a digit,

- negative numbers are written as `-657867654` with the sign `-` followed immediately by a digit immediately followed by a digit,

- spaces to the left or right of = are allowed but not required,

- you can have zero or more spaces at the beginning of each line.

## Contributors

| Nom       | Prénom  | Github            |
|:----------|:--------|:------------------|
| RODRIGUEZ | Lucas   | @ryohkhn          |
| MARTINEAU | Clément | @martineauclement |
