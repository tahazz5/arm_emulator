#include "emulator.h"
#include <iostream>
#include <stdexcept>

RISCVEmulator::RISCVEmulator() : pc(0) {
    memory.resize(1024 * 1024); // 1MB memory
    regs[0] = 0; // x0 is always 0
    for (int i = 1; i < 32; ++i) regs[i] = 0;
}

void RISCVEmulator::loadProgram(const std::vector<uint32_t>& program) {
    for (size_t i = 0; i < program.size(); ++i) {
        uint32_t addr = i * 4;
        memory[addr] = program[i] & 0xFF;
        memory[addr + 1] = (program[i] >> 8) & 0xFF;
        memory[addr + 2] = (program[i] >> 16) & 0xFF;
        memory[addr + 3] = (program[i] >> 24) & 0xFF;
    }
}

void RISCVEmulator::run() {
    while (true) {
        uint32_t instr = fetchInstruction();
        if (instr == 0) break; // Assume 0 is halt or end
        executeInstruction(instr);
    }
}

uint32_t RISCVEmulator::fetchInstruction() {
    if (pc >= memory.size()) throw std::runtime_error("PC out of bounds");
    uint32_t instr = memory[pc] | (memory[pc + 1] << 8) | (memory[pc + 2] << 16) | (memory[pc + 3] << 24);
    pc += 4;
    return instr;
}

void RISCVEmulator::executeInstruction(uint32_t instr) {
    uint32_t opcode = getOpcode(instr);
    switch (opcode) {
        case 0x33: { // R-type
            uint32_t rd = getRd(instr);
            uint32_t rs1 = getRs1(instr);
            uint32_t rs2 = getRs2(instr);
            uint32_t funct3 = (instr >> 12) & 0x7;
            uint32_t funct7 = (instr >> 25) & 0x7F;
            if (funct3 == 0 && funct7 == 0) { // ADD
                regs[rd] = regs[rs1] + regs[rs2];
            } else if (funct3 == 0 && funct7 == 0x20) { // SUB
                regs[rd] = regs[rs1] - regs[rs2];
            } else if (funct3 == 0x7 && funct7 == 0) { // AND
                regs[rd] = regs[rs1] & regs[rs2];
            } else if (funct3 == 0x6 && funct7 == 0) { // OR
                regs[rd] = regs[rs1] | regs[rs2];
            } else if (funct3 == 0x4 && funct7 == 0) { // XOR
                regs[rd] = regs[rs1] ^ regs[rs2];
            } else if (funct3 == 0x2 && funct7 == 0) { // SLT
                regs[rd] = (static_cast<int32_t>(regs[rs1]) < static_cast<int32_t>(regs[rs2])) ? 1 : 0;
            } else {
                std::cerr << "Unsupported R-type instruction" << std::endl;
            }
            break;
        }
        case 0x13: { // I-type
            uint32_t rd = getRd(instr);
            uint32_t rs1 = getRs1(instr);
            int32_t imm = getImmI(instr);
            uint32_t funct3 = (instr >> 12) & 0x7;
            if (funct3 == 0) { // ADDI
                regs[rd] = regs[rs1] + imm;
            } else if (funct3 == 0x7) { // ANDI
                regs[rd] = regs[rs1] & imm;
            } else if (funct3 == 0x6) { // ORI
                regs[rd] = regs[rs1] | imm;
            } else if (funct3 == 0x4) { // XORI
                regs[rd] = regs[rs1] ^ imm;
            } else if (funct3 == 0x2) { // SLTI
                regs[rd] = (static_cast<int32_t>(regs[rs1]) < imm) ? 1 : 0;
            } else {
                std::cerr << "Unsupported I-type instruction" << std::endl;
            }
            break;
        }
        case 0x03: { // Load
            uint32_t rd = getRd(instr);
            uint32_t rs1 = getRs1(instr);
            int32_t imm = getImmI(instr);
            uint32_t addr = regs[rs1] + imm;
            uint32_t funct3 = (instr >> 12) & 0x7;
            if (funct3 == 0x2) { // LW
                regs[rd] = memory[addr] | (memory[addr + 1] << 8) | (memory[addr + 2] << 16) | (memory[addr + 3] << 24);
            } else {
                std::cerr << "Unsupported load instruction" << std::endl;
            }
            break;
        }
        case 0x23: { // Store
            uint32_t rs1 = getRs1(instr);
            uint32_t rs2 = getRs2(instr);
            int32_t imm = getImmS(instr);
            uint32_t addr = regs[rs1] + imm;
            uint32_t funct3 = (instr >> 12) & 0x7;
            if (funct3 == 0x2) { // SW
                memory[addr] = regs[rs2] & 0xFF;
                memory[addr + 1] = (regs[rs2] >> 8) & 0xFF;
                memory[addr + 2] = (regs[rs2] >> 16) & 0xFF;
                memory[addr + 3] = (regs[rs2] >> 24) & 0xFF;
            } else {
                std::cerr << "Unsupported store instruction" << std::endl;
            }
            break;
        }
        case 0x63: { // Branch
            uint32_t rs1 = getRs1(instr);
            uint32_t rs2 = getRs2(instr);
            int32_t imm = getImmB(instr);
            uint32_t funct3 = (instr >> 12) & 0x7;
            bool taken = false;
            if (funct3 == 0) { // BEQ
                taken = (regs[rs1] == regs[rs2]);
            } else if (funct3 == 1) { // BNE
                taken = (regs[rs1] != regs[rs2]);
            } else if (funct3 == 4) { // BLT
                taken = (static_cast<int32_t>(regs[rs1]) < static_cast<int32_t>(regs[rs2]));
            } else if (funct3 == 5) { // BGE
                taken = (static_cast<int32_t>(regs[rs1]) >= static_cast<int32_t>(regs[rs2]));
            } else {
                std::cerr << "Unsupported branch instruction" << std::endl;
            }
            if (taken) {
                pc += imm - 4; // Since pc already incremented
            }
            break;
        }
        case 0x6F: { // JAL
            uint32_t rd = getRd(instr);
            int32_t imm = getImmJ(instr);
            regs[rd] = pc;
            pc += imm - 4;
            break;
        }
        case 0x67: { // JALR
            uint32_t rd = getRd(instr);
            uint32_t rs1 = getRs1(instr);
            int32_t imm = getImmI(instr);
            regs[rd] = pc;
            pc = (regs[rs1] + imm) & ~1;
            break;
        }
        case 0x37: { // LUI
            uint32_t rd = getRd(instr);
            int32_t imm = getImmU(instr);
            regs[rd] = imm;
            break;
        }
        case 0x17: { // AUIPC
            uint32_t rd = getRd(instr);
            int32_t imm = getImmU(instr);
            regs[rd] = pc - 4 + imm;
            break;
        }
        default:
            std::cerr << "Unsupported opcode: " << std::hex << opcode << std::endl;
            break;
    }
    regs[0] = 0; // Ensure x0 is always 0
}

uint32_t RISCVEmulator::getMemoryWord(uint32_t addr) {
    return memory[addr] | (memory[addr + 1] << 8) | (memory[addr + 2] << 16) | (memory[addr + 3] << 24);
}

uint32_t RISCVEmulator::getRegister(uint32_t reg_num) {
    if (reg_num < 32) {
        return regs[reg_num];
    }
    return 0;
}

// Helper functions
uint32_t RISCVEmulator::getOpcode(uint32_t instr) {
    return instr & 0x7F;
}

uint32_t RISCVEmulator::getRd(uint32_t instr) {
    return (instr >> 7) & 0x1F;
}

uint32_t RISCVEmulator::getRs1(uint32_t instr) {
    return (instr >> 15) & 0x1F;
}

uint32_t RISCVEmulator::getRs2(uint32_t instr) {
    return (instr >> 20) & 0x1F;
}

int32_t RISCVEmulator::getImmI(uint32_t instr) {
    return static_cast<int32_t>(instr) >> 20;
}

int32_t RISCVEmulator::getImmS(uint32_t instr) {
    int32_t imm = ((instr >> 7) & 0x1F) | ((instr >> 25) & 0x7F) << 5;
    return (imm << 20) >> 20; // Sign extend
}

int32_t RISCVEmulator::getImmB(uint32_t instr) {
    int32_t imm = ((instr >> 8) & 0xF) << 1 | ((instr >> 25) & 0x3F) << 5 | ((instr >> 7) & 0x1) << 11 | ((instr >> 31) & 0x1) << 12;
    return (imm << 19) >> 19; // Sign extend
}

int32_t RISCVEmulator::getImmU(uint32_t instr) {
    return instr & 0xFFFFF000;
}

int32_t RISCVEmulator::getImmJ(uint32_t instr) {
    int32_t imm = ((instr >> 21) & 0x3FF) << 1 | ((instr >> 20) & 0x1) << 11 | ((instr >> 12) & 0xFF) << 12 | ((instr >> 31) & 0x1) << 20;
    return (imm << 11) >> 11; // Sign extend
}