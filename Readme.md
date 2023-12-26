# Compiler Project

This project encompasses various phases of a compiler, each residing in its respective directory.
## Directory Structure

### 01Lexical/
Contains the implementation of the lexical analysis phase of the compiler.

- **File:** `lexical.cpp`
  - Reads the `input.c` file and generates tokens.
  - Tokens are written to `output.txt`.
  - Performs bracket matching using the `chck_brakects` function.

### 02SyntaxTree/
Houses the syntax tree generation phase of the compiler.

- **Files:**
  - `SyntaxPhase.cpp`: Parses `While.c` and generates a syntax tree.
  - `SyntaxPhase.hpp`: Contains functions for parsing different code structures.

### 03LRParser/
Encompasses the LR parsing phase of the compiler.

- **File:** `shiftreduce.cpp`
  - Reads `input.c` and `rules.txt`.
  - Performs shift-reduce parsing and writes the output to `Output.txt`.
  - Utilizes a set of production rules defined in `rules.txt`.

## How to Run

To execute specific phases of the compiler:

- Navigate to the relevant directory (e.g., `01Lexical/`, `02SyntaxTree/`, or `03LRParser/`).
- Compile and run the corresponding source file (e.g., `lexical.cpp`, `SyntaxPhase.cpp`, or `shiftreduce.cpp`).

For example:
```bash
cd Compiler/01Lexical/
g++ lexical.cpp -o lexicalAnalyzer
./lexicalAnalyzer
```

## Dependencies

- C++ compiler for compiling the source files.
- Input files such as `input.c`, `While.c`, and `rules.txt` as required by specific phases.

## Additional Notes

- The lexical analysis phase checks for balanced brackets using the `chck_brakects` function.
- The LR parsing phase utilizes a set of production rules defined in `rules.txt`.

Feel free to explore each directory for detailed implementation and outputs. For any questions or issues, refer to the specific README within each directory.

Happy compiling!