#ifndef EMULATOR_H
#define EMULATOR_H

#include <cstdint>
#include <vector>

class RISCVEmulator {
public:
    RISCVEmulator();
    void loadProgram(const std::vector<uint32_t>& program);
    void run();
    uint32_t getMemoryWord(uint32_t addr);
    uint32_t getRegister(uint32_t reg_num);

private:
    // Registers: x0 is always 0, x1-x31 general purpose
    uint32_t regs[32];
    uint32_t pc; // Program counter
    std::vector<uint8_t> memory; // Memory

    // Instruction execution
    void executeInstruction(uint32_t instr);
    uint32_t fetchInstruction();

    // Helper functions for instruction decoding
    uint32_t getOpcode(uint32_t instr);
    uint32_t getRd(uint32_t instr);
    uint32_t getRs1(uint32_t instr);
    uint32_t getRs2(uint32_t instr);
    int32_t getImmI(uint32_t instr);
    int32_t getImmS(uint32_t instr);
    int32_t getImmB(uint32_t instr);
    int32_t getImmU(uint32_t instr);
    int32_t getImmJ(uint32_t instr);
};

#endif // EMULATOR_H