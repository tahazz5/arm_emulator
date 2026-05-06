#include "emulator.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

class InteractiveEmulator {
private:
    RISCVEmulator emu;
    std::vector<uint32_t> program;
    bool running;

public:
    InteractiveEmulator() : running(true) {}

    void run() {
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║  RISC-V Emulator - Interactive Mode    ║\n";
        std::cout << "╚════════════════════════════════════════╝\n\n";
        printHelp();

        std::string command;
        while (running) {
            std::cout << "\n> ";
            std::getline(std::cin, command);
            processCommand(command);
        }
    }

private:
    void processCommand(const std::string& cmd) {
        std::istringstream iss(cmd);
        std::string action;
        iss >> action;

        if (action.empty()) return;

        if (action == "help" || action == "h") {
            printHelp();
        } else if (action == "load" || action == "l") {
            loadProgramInteractive(iss);
        } else if (action == "run" || action == "r") {
            runProgram();
        } else if (action == "regs" || action == "reg") {
            printRegisters();
        } else if (action == "mem" || action == "m") {
            printMemory(iss);
        } else if (action == "clear" || action == "c") {
            clearAll();
        } else if (action == "example" || action == "ex") {
            int exNum = 1;
            iss >> exNum;
            loadExample(exNum);
        } else if (action == "exit" || action == "quit" || action == "q") {
            running = false;
            std::cout << "Au revoir!\n";
        } else {
            std::cout << "Commande inconnue: " << action << ". Tapez 'help' pour l'aide.\n";
        }
    }

    void loadProgramInteractive(std::istringstream& iss) {
        program.clear();
        std::cout << "Entrez les instructions en hex (ex: 0x02A00093). Tapez 'end' pour terminer:\n";
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line == "end" || line.empty()) break;
            try {
                uint32_t instr = std::stoul(line, nullptr, 16);
                program.push_back(instr);
                std::cout << "Instruction ajoutée: 0x" << std::hex << std::setw(8) 
                         << std::setfill('0') << instr << std::dec << "\n";
            } catch (...) {
                std::cout << "Format invalide. Utilisez le format hex (ex: 0x02A00093)\n";
            }
        }
        if (!program.empty()) {
            program.push_back(0x00000000); // Ajout du halt
            std::cout << "Programme chargé avec " << program.size() - 1 << " instructions.\n";
        }
    }

    void loadExample(int exNum) {
        program.clear();
        switch (exNum) {
            case 1: {
                // Exemple 1: 42 + 1 = 43
                program = {
                    0x02A00093, // ADDI x1, x0, 42
                    0x00100113, // ADDI x2, x0, 1
                    0x002081B3, // ADD x3, x1, x2
                    0x00302023, // SW x3, 0(x0)
                    0x00000000  // Halt
                };
                std::cout << "Exemple 1 chargé: Calcul 42 + 1\n";
                break;
            }
            case 2: {
                // Exemple 2: 100 - 30 = 70
                program = {
                    0x06400093, // ADDI x1, x0, 100
                    0x01E00113, // ADDI x2, x0, 30
                    0x402081B3, // SUB x3, x1, x2
                    0x00302023, // SW x3, 0(x0)
                    0x00000000  // Halt
                };
                std::cout << "Exemple 2 chargé: Calcul 100 - 30\n";
                break;
            }
            case 3: {
                // Exemple 3: Opérations logiques
                program = {
                    0x0FF00093, // ADDI x1, x0, 255 (0xFF)
                    0x00F00113, // ADDI x2, x0, 15 (0x0F)
                    0x0020F1B3, // AND x3, x1, x2
                    0x00502223, // SW x3, 4(x0)
                    0x0020E1B3, // OR x3, x1, x2 (reuse pour OR)
                    0x00802223, // SW x3, 8(x0)
                    0x0020C1B3, // XOR x3, x1, x2 (reuse pour XOR)
                    0x00B02223, // SW x3, 12(x0)
                    0x00000000  // Halt
                };
                std::cout << "Exemple 3 chargé: Opérations logiques (AND, OR, XOR)\n";
                break;
            }
            default:
                std::cout << "Exemple pas trouvé. Exemples disponibles: 1, 2, 3\n";
                return;
        }
    }

    void runProgram() {
        if (program.empty()) {
            std::cout << "Aucun programme chargé. Chargez un programme d'abord (load ou example).\n";
            return;
        }
        try {
            RISCVEmulator tempEmu;
            tempEmu.loadProgram(program);
            tempEmu.run();
            emu = tempEmu;
            std::cout << "Programme exécuté avec succès!\n";
        } catch (const std::exception& e) {
            std::cout << "Erreur lors de l'exécution: " << e.what() << "\n";
        }
    }

    void printRegisters() {
        std::cout << "\n╔═══════════════════════════════════════╗\n";
        std::cout << "║          REGISTRES (RV32I)            ║\n";
        std::cout << "╠═══════════════════════════════════════╣\n";
        for (int i = 0; i < 32; i++) {
            uint32_t val = emu.getRegister(i);
            if (val != 0 || i == 0) {
                std::cout << "║ x" << std::setw(2) << std::setfill('0') << i 
                         << " = 0x" << std::hex << std::setw(8) << std::setfill('0') 
                         << val << std::dec << "  (" << std::setw(10) << val << ")    ║\n";
            }
        }
        std::cout << "╚═══════════════════════════════════════╝\n";
    }

    void printMemory(std::istringstream& iss) {
        uint32_t addr = 0;
        iss >> std::hex >> addr;
        uint32_t size = 16;
        iss >> std::hex >> size;

        std::cout << "\n╔═══════════════════════════════════════╗\n";
        std::cout << "║ MÉMOIRE @ 0x" << std::hex << std::setw(6) << std::setfill('0') 
                 << addr << std::dec << "                    ║\n";
        std::cout << "╠═══════════════════════════════════════╣\n";

        for (uint32_t i = 0; i < size; i += 4) {
            uint32_t word = emu.getMemoryWord(addr + i);
            std::cout << "║ @0x" << std::hex << std::setw(8) << std::setfill('0') 
                     << (addr + i) << std::dec << ": 0x" << std::hex << std::setw(8) 
                     << std::setfill('0') << word << "  (" << std::setw(10) << word << ") ║\n";
        }
        std::cout << "╚═══════════════════════════════════════╝\n";
    }

    void clearAll() {
        program.clear();
        emu = RISCVEmulator();
        std::cout << "Tous les données ont été effacées.\n";
    }

    void printHelp() {
        std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                       COMMANDES DISPONIBLES                      ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  load, l          - Charger un programme (saisie manuelle)      ║\n";
        std::cout << "║  example, ex N    - Charger un exemple (1, 2, ou 3)             ║\n";
        std::cout << "║  run, r           - Exécuter le programme chargé                ║\n";
        std::cout << "║  regs, reg        - Afficher tous les registres                 ║\n";
        std::cout << "║  mem, m [addr]    - Afficher la mémoire (défaut: 0x0)          ║\n";
        std::cout << "║  clear, c         - Effacer le programme et réinitialiser       ║\n";
        std::cout << "║  help, h          - Afficher cette aide                         ║\n";
        std::cout << "║  exit, quit, q    - Quitter le programme                       ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    }
};

int main() {
    InteractiveEmulator interactiveEmu;
    interactiveEmu.run();
    return 0;
}