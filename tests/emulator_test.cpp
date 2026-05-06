#include "gtest/gtest.h"
#include "emulator.h"
#include <vector>

class RISCVEmulatorTest : public ::testing::Test {
protected:
    RISCVEmulator emu;
};

// Test ADDI instruction
TEST_F(RISCVEmulatorTest, ADDIInstruction) {
    std::vector<uint32_t> program = {
        0x02A00093, // ADDI x1, x0, 42
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    EXPECT_EQ(emu.getRegister(1), 42);
}

// Test ADD instruction
TEST_F(RISCVEmulatorTest, ADDInstruction) {
    std::vector<uint32_t> program = {
        0x02A00093, // ADDI x1, x0, 42
        0x00100113, // ADDI x2, x0, 1
        0x002081B3, // ADD x3, x1, x2
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    EXPECT_EQ(emu.getRegister(3), 43);
}

// Test SUB instruction
TEST_F(RISCVEmulatorTest, SUBInstruction) {
    std::vector<uint32_t> program = {
        0x05000093, // ADDI x1, x0, 80
        0x00A00113, // ADDI x2, x0, 10
        0x402081B3, // SUB x3, x1, x2
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    EXPECT_EQ(emu.getRegister(3), 70);
}

// Test LW/SW instructions
TEST_F(RISCVEmulatorTest, LoadStoreWord) {
    std::vector<uint32_t> program = {
        0x02A00093, // ADDI x1, x0, 42
        0x00100113, // ADDI x2, x0, 1
        0x002081B3, // ADD x3, x1, x2
        0x00302023, // SW x3, 0(x0)
        0x0000283B, // LW x5, 0(x0) -- Note: Will need proper offset
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    // Verify store worked
    EXPECT_EQ(emu.getMemoryWord(0), 43);
}

// Test x0 is always 0
TEST_F(RISCVEmulatorTest, X0AlwaysZero) {
    std::vector<uint32_t> program = {
        0x02A00093, // ADDI x1, x0, 42 (x0 stays 0)
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    EXPECT_EQ(emu.getRegister(0), 0);
}

// Test AND instruction
TEST_F(RISCVEmulatorTest, ANDInstruction) {
    std::vector<uint32_t> program = {
        0x0FF00093, // ADDI x1, x0, 255 (0xFF)
        0x00F00113, // ADDI x2, x0, 15 (0x0F)
        0x0020F1B3, // AND x3, x1, x2
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    EXPECT_EQ(emu.getRegister(3), 0x0F);
}

// Test OR instruction
TEST_F(RISCVEmulatorTest, ORInstruction) {
    std::vector<uint32_t> program = {
        0x0F000093, // ADDI x1, x0, 240 (0xF0)
        0x00F00113, // ADDI x2, x0, 15 (0x0F)
        0x0020E1B3, // OR x3, x1, x2
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    EXPECT_EQ(emu.getRegister(3), 0xFF);
}

// Test XOR instruction
TEST_F(RISCVEmulatorTest, XORInstruction) {
    std::vector<uint32_t> program = {
        0x0FF00093, // ADDI x1, x0, 255 (0xFF)
        0x0FF00113, // ADDI x2, x0, 255 (0xFF)
        0x0020C1B3, // XOR x3, x1, x2
        0x00000000  // Halt
    };
    emu.loadProgram(program);
    emu.run();
    EXPECT_EQ(emu.getRegister(3), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}