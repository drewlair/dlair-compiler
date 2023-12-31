2023 B-Minor Compiler - Created by Drew Lair
-----------------------------------

This repository contains the compiler code for the 2023 version of the B-Minor Language.
B-Minor is a imperative language with a strict static type system that supports standard 
language features including complex expressions, basic control flow, and complete
functionality of the C standard library. A more complete description can be found here: [https://dthain.github.io/compilers-fa23/bminor.html](https://dthain.github.io/compilers-fa23/bminor.html).

Please move into the main directory and run _make_ to compile the B-Minor compiler. 
The compiler is split into 6 distinct steps: Scanning, Parsing, Printing, Name Resolution, Typechecking, and Code generation.

## Scanning
The Flex program is used to tokenize the words of our BMinor program using a regular expression matcher. Run the scanner using:
```
./exec/bminor --scan sourcefile.bminor
```

## Parsing
Using Bison, I set up a Context Free Grammar that constructs valid phrases and sentences and tokens. Run the parser using:
```
./exec/bminor --parse sourcefile.bminor
```

## Printer
Pretty prints (reformats) the bminor file. Run the printer using: 
```
./exec/bminor --print sourcefile.bminor
```

## Name Resolution
Checks that identifiers are not redeclared within the same context and functions are not redeclared with code. Run name resolution using:
```
./exec/bminor --resolve sourcefile.bminor
```

## TypeChecker
Checks that there are no type mismatches with declared types (due to strict type checking and static typing). Run the typechecker using:
```
./exec/bminor --typecheck sourcefile.bminor
```

## Code Generation
If all previous steps pass, an x86 assembly file is generated that can be compiled via gcc. Run the code generator using:
```
./exec/bminor --codegen sourcefile.bminor > sourcefile.s
```

## Executable
A final executable can be generated by running:
```
gcc -g sourcefile.s library.c -o myprogram
```
This executable can be run using:
```
./myprogram
```

## Testing
To run some basic tests for each part of the system, run _make test_ in the home directory. This will test the final codegenerator with 30
custom tests. If you plan to make changes for specific phases of the compiler, you may also run _make_ with the following keywords that correspoind to the compiler component:
scan parse print resolve typecheck codegen
Each will run a set of tests for the specific components and complete log files that describe the program results in more detail (these files are located in the test/_componentName_ folder and end with _.out_).

## Current Version Notes
The current version of this compiler does not support floats or the declaration of multi-dimensional arrays. It is also important to note that 
arrays cannot be declared locally inside functions, only in global scope. I plan to add compatibility for both features in the near future
and will create new releases when the necessary changes are complete.

