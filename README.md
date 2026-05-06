# RISC-V Emulator in C++

This is a simple RISC-V (RV32I) emulator implemented in C++.

## Features

- Supports basic RV32I instructions: ADD, SUB, AND, OR, XOR, SLT, ADDI, ANDI, ORI, XORI, SLTI, LW, SW, BEQ, BNE, BLT, BGE, JAL, JALR, LUI, AUIPC
- 32 general-purpose registers
- 1MB memory
- Program counter
- Unit tests with Google Test
- Interactive CLI for testing and debugging

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

### Main Emulator (Test Only)

```bash
./emulator
```

The emulator runs a simple test program that computes 42 + 1 = 43 and stores it in memory at address 0.

### Interactive Emulator

```bash
./emulator_interactive
```

The interactive mode allows you to:
- Load custom programs by entering instructions in hexadecimal
- Load pre-built examples (3 examples available)
- Run programs and inspect results
- View all registers
- View memory contents
- Clear all data and start fresh

**Interactive Commands:**
- `load` or `l` - Load a program (manual hex input)
- `example` or `ex [1-3]` - Load a pre-built example
- `run` or `r` - Execute the loaded program
- `regs` or `reg` - Display all registers
- `mem` or `m [address]` - Display memory
- `clear` or `c` - Reset everything
- `help` or `h` - Show help
- `exit` or `quit` or `q` - Exit the program

### Unit Tests

```bash
./emulator_tests
```

Runs 8 unit tests covering:
- ADDI (Add Immediate)
- ADD (Add)
- SUB (Subtract)
- Load/Store (LW/SW)
- X0 register constraint
- AND, OR, XOR operations

All tests should pass ✓

## Usage Example

### Interactive Mode

```
> example ex 1
Exemple 1 chargé: Calcul 42 + 1
> run
Programme exécuté avec succès!
> regs
[Displays all registers with x3 = 43]
> mem 0
[Displays memory at address 0 with value 43]
```

### Loading Custom Program

```
> load
Entrez les instructions en hex (ex: 0x02A00093). Tapez 'end' pour terminer:
0x02A00093
Instruction ajoutée: 0x02a00093
0x00100113
Instruction ajoutée: 0x00100113
0x002081B3
Instruction ajoutée: 0x002081b3
end
Programme chargé avec 3 instructions.
> run
Programme exécuté avec succès!
```

## Project Structure

```
├── include/
│   └── emulator.h       # Emulator class definition
├── src/
│   ├── emulator.cpp     # Emulator implementation
│   ├── main.cpp         # Simple test program
│   └── interactive.cpp  # Interactive CLI
├── tests/
│   └── emulator_test.cpp  # Unit tests
├── CMakeLists.txt       # Build configuration
└── README.md
```

## RISC-V Instruction Examples

### Arithmetic
- `0x02A00093` - ADDI x1, x0, 42 (Add 42 to register x0, store in x1)
- `0x002081B3` - ADD x3, x1, x2 (Add x1 and x2, store in x3)
- `0x402081B3` - SUB x3, x1, x2 (Subtract x2 from x1, store in x3)

### Logical Operations
- `0x0020F1B3` - AND x3, x1, x2
- `0x0020E1B3` - OR x3, x1, x2
- `0x0020C1B3` - XOR x3, x1, x2

### Memory Operations
- `0x00302023` - SW x3, 0(x0) (Store word x3 to memory at address 0)
- `0x00002283` - LW x5, 0(x0) (Load word from memory at address 0 to x5)