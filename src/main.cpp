#include "emulator.h"
#include <iostream>
#include <vector>

int main() {
    RISCVEmulator emu;

    // Simple program: load 42 into x1, add 1, store to memory at 0
    // ADDI x1, x0, 42
    // ADDI x2, x0, 1
    // ADD x3, x1, x2
    // SW x3, 0(x0)

    std::vector<uint32_t> program = {
        0x02A00093, // ADDI x1, x0, 42
        0x00100113, // ADDI x2, x0, 1
        0x002081B3, // ADD x3, x1, x2
        0x00302023, // SW x3, 0(x0)
        0x00000000  // Halt (0)
    };

    emu.loadProgram(program);
    emu.run();

    // Check memory at 0
    uint32_t result = emu.getMemoryWord(0);
    std::cout << "Result in memory: " << result << std::endl; // Should be 43

    return 0;
}