# Helix Compiler

A lightweight educational compiler implementation for the **Helix programming language** (`.hxi`), written in C. This project demonstrates the fundamental phases of compilation: lexical analysis, parsing, intermediate code generation, and optimization.

## 🚀 Overview

Helix Compiler is a multi-phase compiler that processes source files with the `.hxi` extension through a complete compilation pipeline. The compiler currently supports arithmetic expression compilation with plans to expand to a full-featured language.

### Current Capabilities
- **Lexical Analysis**: Tokenizes source code into keywords, identifiers, operators, and numbers
- **Syntax Analysis**: Builds Abstract Syntax Trees (AST) using recursive descent parsing
- **Semantic Analysis**: Evaluates expressions and validates syntax
- **Intermediate Code Generation**: Produces Three-Address Code (TAC)
- **Optimization**: DAG-based common subexpression elimination

## 📋 Features

### Implemented
- ✅ Multi-character operator recognition (`==`, `+=`, `-=`, `*=`, `/=`, `>=`, `<=`)
- ✅ Single-line comment support (`//`)
- ✅ Expression parsing with operator precedence (addition and multiplication)
- ✅ AST construction and evaluation
- ✅ Three-Address Code (TAC) generation
- ✅ DAG-based optimization to eliminate redundant computations
- ✅ File I/O with `.hxi` extension validation

### Language Support
**Keywords**: `int`, `if`, `while`, `return`, `main`  
**Operators**: `+`, `-`, `*`, `/`, `=`, `==`, `+=`, `-=`, `*=`, `/=`, `>=`, `<=`, `;`, `(`, `)`, `{`, `}`, `<`, `>`

## 🛠️ Installation

### Prerequisites
- GCC compiler or any C compiler
- Unix-like environment (Linux, macOS) or Windows with MinGW/Cygwin

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/muditagrawal-alt/Helix-Compiler.git
cd Helix-Compiler

# Compile the compiler
gcc COMPILER.C -o helix

# Or use the individual compilation approach
gcc lexer.c parser.c tac.c optimizer.c COMPILER.C -o helix
```

## 📖 Usage

### Basic Usage

```bash
# Run with default test file (test.hxi)
./helix

# Run with a specific .hxi file
./helix yourfile.hxi
```

### Example Input (`test.hxi`)

```
2*3+2*3;
```

### Expected Output

```
--- HELIX COMPILER ---
Source file: test.hxi

--- TOKENS ---
Token: 2 -> NUMBER
Token: * -> OPERATOR
Token: 3 -> NUMBER
Token: + -> OPERATOR
Token: 2 -> NUMBER
Token: * -> OPERATOR
Token: 3 -> NUMBER
Token: ; -> OPERATOR

--- EXPRESSION SENT TO PARSER ---
2*3+2*3

--- PARSER / EVALUATOR RESULT ---
Result: 12

--- THREE ADDRESS CODE ---
t1 = 2 * 3
t2 = 2 * 3
t3 = t1 + t2

--- OPTIMIZED TAC (DAG PROTOTYPE) ---
t1 = 2 * 3
t2 = t1
t3 = t1 + t2
```

Notice how the optimizer recognizes that `2*3` is computed twice and reuses the result.

## 🏗️ Architecture

### Compilation Pipeline

```
┌──────────────┐
│  Source Code │
│   (.hxi)     │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    LEXER     │  ─── Tokenization
│  (lexer.c)   │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    PARSER    │  ─── AST Construction
│  (parser.c)  │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│      TAC     │  ─── Intermediate Code
│   (tac.c)    │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│  OPTIMIZER   │  ─── DAG-based CSE
│(optimizer.c) │
└──────────────┘
```

### File Structure

```
Helix-Compiler/
├── COMPILER.C      # Main driver program
├── lexer.c         # Lexical analyzer
├── parser.c        # Syntax analyzer and AST builder
├── tac.c           # Three-Address Code generator
├── optimizer.c     # Code optimization (DAG)
├── test.hxi        # Sample Helix source file
├── helix           # Compiled executable
├── optimizer       # Optimizer executable
├── tac             # TAC generator executable
└── LICENSE         # MIT License
```

### Core Data Structures

**AST Node**:
```c
typedef struct Node {
    char value;
    struct Node* left;
    struct Node* right;
} Node;
```

**TAC Format**:
```
temp = operand1 operator operand2
```

## 🔧 Compiler Phases Explained

### 1. Lexical Analysis
The lexer scans the source file character by character and groups them into meaningful tokens:
- **Keywords**: Reserved words like `int`, `if`, `while`
- **Identifiers**: Variable and function names
- **Operators**: Arithmetic and logical operators
- **Numbers**: Integer literals
- **Special Characters**: Semicolons, parentheses, braces

### 2. Syntax Analysis
The parser uses **recursive descent parsing** to build an Abstract Syntax Tree. Grammar rules:
```
E → T + E | T
T → F * T | F
F → (E) | number
```

### 3. Intermediate Code Generation
Generates three-address code (TAC), a low-level representation closer to assembly:
```
t1 = a + b
t2 = t1 * c
```

### 4. Optimization
The optimizer builds a **Directed Acyclic Graph (DAG)** to identify and eliminate common subexpressions, reducing redundant computations.

## 📊 Examples

### Example 1: Simple Expression
**Input** (`example1.hxi`):
```
3+4*2;
```

**Output**:
```
Result: 11
t1 = 4 * 2
t2 = 3 + t1
```

### Example 2: Parenthesized Expression
**Input** (`example2.hxi`):
```
(2+3)*4;
```

**Output**:
```
Result: 20
t1 = 2 + 3
t2 = t1 * 4
```

### Example 3: Common Subexpression
**Input** (`example3.hxi`):
```
5*6+5*6;
```

**Optimized Output**:
```
t1 = 5 * 6
t2 = t1      // Reuses t1 instead of recalculating 5*6
t3 = t1 + t2
```

## 🎯 Roadmap

### Planned Features
- [ ] Variable declarations and assignments
- [ ] Control flow structures (`if`, `while`, `for`)
- [ ] Function definitions and calls
- [ ] Array support
- [ ] Type checking and semantic analysis
- [ ] Symbol table implementation
- [ ] Machine code generation (x86-64 assembly)
- [ ] More sophisticated optimizations (constant folding, dead code elimination)
- [ ] Error reporting with line numbers and helpful messages
- [ ] Support for floating-point arithmetic
- [ ] String literals and operations
- [ ] Standard library functions

### Long-term Vision
- Multi-file compilation and linking
- Preprocessor directives
- Struct/record types
- Pointer arithmetic
- Module system
- LLVM backend integration

## 🧪 Testing

Create your own test files with the `.hxi` extension:

```bash
echo "10+20*3;" > mytest.hxi
./helix mytest.hxi
```

**Note**: Currently, only arithmetic expressions are fully supported. Other language constructs will be recognized during lexing but may not be processed in later stages.

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow the existing code style
- Add comments for complex logic
- Test with various input expressions
- Update documentation for new features

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


